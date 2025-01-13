import asyncio
from pathlib import Path
from typing import TYPE_CHECKING

import pytest
from farm_ng.core.event_pb2 import Event
from farm_ng.core.event_service import EventServiceGrpc
from farm_ng.core.event_service_pb2 import (
    EventServiceConfigList,
    RequestReplyRequest,
)
from farm_ng.core.event_service_recorder import EventServiceRecorder, RecorderService
from farm_ng.core.events_file_reader import EventsFileReader, payload_to_protobuf
from farm_ng.core.uri import make_proto_uri
from google.protobuf.message import Message
from google.protobuf.wrappers_pb2 import Int32Value

if TYPE_CHECKING:
    from farm_ng.core.uri_pb2 import Uri

pytestmark = pytest.mark.anyio


async def request_reply_handler(
    request: RequestReplyRequest,
) -> Message:
    message = payload_to_protobuf(request.event, request.payload)
    print(
        f"Received: {request.event.uri.path} {request.event.sequence} {message}".rstrip(),
    )
    return message  # echo message back


class TestEventServiceRecorder:
    def test_smoke(self, recorder_service: EventServiceRecorder) -> None:
        assert recorder_service is not None
        assert recorder_service.QUEUE_MAX_SIZE == 50
        assert recorder_service.service_name == "record_default"
        assert recorder_service.logger.name == "record_default"
        assert recorder_service.record_queue.qsize() == 0

    async def test_event_service_recorder(
        self,
        tmp_path: Path,
        event_service: EventServiceGrpc,
        recorder_service: EventServiceRecorder,
    ) -> None:
        # reset the counts
        event_service.reset()
        event_service.request_reply_handler = request_reply_handler

        # start the subcriber and record
        file_name = tmp_path / "test_record"
        task = asyncio.create_task(
            recorder_service.subscribe_and_record(file_name),
        )
        await asyncio.sleep(0.1)

        assert "/foo" in event_service._client_queues
        assert "/bar" in event_service._client_queues

        # publish a message

        message = Int32Value(value=3)

        res = await event_service.publish(path="/foo", message=message)

        assert res.number_clients == 1
        assert res.sequence_number == 0

        # to verify that the message was recorded, we need to wait for the
        await asyncio.sleep(0.1)

        task.cancel()
        # to make sure that the task is cancelled
        await asyncio.sleep(0.1)

        file_name_bin = file_name.with_suffix(".0000.bin")
        assert file_name_bin.exists()

        # read the file
        reader = EventsFileReader(file_name_bin)
        assert reader.open()

        received_msg: bool = False
        for event_log in reader.get_index():
            event_message = event_log.read_message()
            assert event_message == message
            received_msg = True
        assert received_msg

    async def test_file_headers(
        self,
        tmp_path: Path,
        event_service: EventServiceGrpc,
        recorder_service: EventServiceRecorder,
    ) -> None:
        # reset the counts
        event_service.reset()
        event_service.request_reply_handler = request_reply_handler

        # Define some test values
        header_uri_base: str = "baz_header"
        header_count: int = 3

        # Create the header and pass it to EventServiceRecorder
        for i in range(10, 10 + header_count):
            message = Int32Value(value=i)
            payload: bytes = message.SerializeToString()
            uri: Uri = make_proto_uri(
                path=f"/{header_uri_base}_{i}",
                message=message,
                service_name=event_service.config.name,
            )
            event = Event(
                uri=uri,
                timestamps=[],
                payload_length=len(payload),
                sequence=i,
            )
            recorder_service.header_deque.append((event, payload))

        # start the recording
        file_name = tmp_path / "test_headers"
        task = asyncio.create_task(
            recorder_service.subscribe_and_record(file_name),
        )
        await asyncio.sleep(0.1)

        # Cancel the recording
        task.cancel()
        await asyncio.sleep(0.1)

        file_name_bin = file_name.with_suffix(".0000.bin")
        assert file_name_bin.exists()

        # read the file
        reader = EventsFileReader(file_name_bin)
        assert reader.open()

        # Check the headers
        logged_headers: int = 0
        for event_log in reader.get_index():
            # Check headers - skip any other events
            if header_uri_base in event_log.event.uri.path:
                logged_headers += 1
                event_message = event_log.read_message()
                assert isinstance(event_message, Int32Value)
        assert logged_headers == header_count


class TestRecorderService:
    async def test_recorder_service(
        self,
        tmp_path: Path,
        event_service_config_list: EventServiceConfigList,
        event_service: EventServiceGrpc,
    ) -> None:
        # reset the counts
        event_service.reset()
        event_service.config.args.extend(["--data-dir", str(tmp_path)])

        # create the recorder service
        recorder_service = RecorderService(event_service)

        file_name = tmp_path / "test_recorder_service"

        await recorder_service.start_recording(
            file_base=file_name,
            config_list=event_service_config_list,
            config_name="record_default",
        )
        await asyncio.sleep(0.1)

        # publish a message
        message = Int32Value(value=3)
        await event_service.publish(path="/foo", message=message)
        await asyncio.sleep(0.1)

        await recorder_service.stop_recording()
        await asyncio.sleep(0.1)

        file_name_bin = file_name.with_suffix(".0000.bin")
        assert file_name_bin.exists()

        # read the file
        reader = EventsFileReader(file_name_bin)
        assert reader.open()

        for event_log in reader.get_index():
            event_message = event_log.read_message()
            assert event_message == message
