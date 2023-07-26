import pytest
import asyncio
from pathlib import Path
import grpc
from google.protobuf.wrappers_pb2 import Int32Value
from google.protobuf.message import Message

from farm_ng.core.events_file_reader import EventsFileReader, payload_to_protobuf
from farm_ng.core.event_service import EventServiceConfigList, EventServiceGrpc
from farm_ng.core.event_service_recorder import EventServiceRecorder
from farm_ng.core.event_service_pb2 import (
    RequestReplyRequest,
)
from .event_common import event_service_config_list


async def request_reply_handler(
    event_service: EventServiceGrpc, request: RequestReplyRequest
) -> Message:
    message = payload_to_protobuf(request.event, request.payload)
    event_service.logger.info(
        f"Received: {request.event.uri.path} {request.event.sequence} {message}".rstrip()
    )
    return message  # echo message back


class TestEventServiceRecorder:
    def test_smoke(self) -> None:
        config_list: EventServiceConfigList = event_service_config_list()
        recorder_service = EventServiceRecorder(
            service_name="recorder_default", config_list=config_list
        )

        assert recorder_service is not None
        assert recorder_service.QUEUE_MAX_SIZE == 50
        assert recorder_service.service_name == "recorder_default"
        assert len(recorder_service.clients) == 1
        assert recorder_service.config_list == config_list
        assert recorder_service.recorder_config == config_list.configs[0]
        assert recorder_service.logger.name == "recorder_default"
        assert recorder_service.record_queue.qsize() == 0

    @pytest.mark.asyncio
    async def test_record(self, tmp_path: Path) -> None:
        config_list: EventServiceConfigList = event_service_config_list()

        event_service = EventServiceGrpc(grpc.aio.server(), config_list.configs[0])
        event_service.request_reply_handler = request_reply_handler

        recorder_service = EventServiceRecorder(
            service_name="recorder_default", config_list=config_list
        )

        # start the event service
        asyncio.create_task(event_service.serve())

        # start the subcriber and record
        file_name = tmp_path / "test_record"
        task = asyncio.create_task(
            # TODO: parametrize the test
            recorder_service.subscribe_and_record(file_name)
        )

        # we need to wait for the subscribe callback to be called
        await asyncio.sleep(0.001)
        await asyncio.sleep(0.001)

        assert "/foo" in event_service._client_queues

        # publish a message

        message = Int32Value(value=3)

        res = await event_service.publish(path="/foo", message=message)

        assert res.number_clients == 1
        assert res.sequence_number == 0

        # to verify that the message was recorded, we need to wait for the
        await asyncio.sleep(0.001)

        task.cancel()
        # to make sure that the task is cancelled
        await asyncio.sleep(0.001)

        file_name_bin = file_name.with_suffix(".0000.bin")
        assert file_name_bin.exists()

        # read the file
        reader = EventsFileReader(file_name_bin)
        assert reader.open()

        for event_log in reader.get_index():
            event_message = event_log.read_message()
            assert event_message == message
