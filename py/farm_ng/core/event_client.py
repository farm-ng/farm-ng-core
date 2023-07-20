import logging
import grpc
from farm_ng.core import event_service_pb2_grpc
import asyncio
from farm_ng.core.stamp import get_monotonic_now, StampSemantics
from farm_ng.core.uri_pb2 import Uri
from farm_ng.core.event_service_pb2 import (
    EventServiceConfig,
    ServiceState as ServiceStatePb,
    SubscribeReply,
    SubscribeRequest,
    ListUrisRequest,
    GetServiceStateRequest,
    GetServiceStateReply,
)
from farm_ng.core.events_file_reader import _parse_protobuf_descriptor
import importlib
from google.protobuf.message import Message

logging.basicConfig(level=logging.INFO)


class ServiceState:
    """Generic service state.

    Possible state values:
        - UNKNOWN: undefined state.
        - RUNNING: the service is up AND streaming.
        - IDLE: the service is up AND NOT streaming.
        - UNAVAILABLE: the service is not available.
        - ERROR: the service is an error state.

    Args:
        proto (ServiceState): protobuf message containing the service state.
    """

    def __init__(self, proto: ServiceStatePb = None) -> None:
        self._proto = ServiceStatePb.UNKNOWN
        if proto is not None:
            self._proto = proto

    @property
    def value(self) -> int:
        """Returns the state enum value."""
        return self._proto

    @property
    def name(self) -> str:
        """Return the state name."""
        return ServiceStatePb.DESCRIPTOR.values[self.value].name

    def __repr__(self) -> str:
        return f"{self.__class__.__name__}: ({self.value}, {self.name})"


class EventClient:
    """Generic client.

    Generic client class to connect with the Amiga brain services.
    Internally implements an `asyncio` gRPC channel.
    Designed to be imported by service specific clients.

    Args:
        config (EventServiceConfig): the grpc configuration data structure.
    """

    def __init__(self, config: EventServiceConfig) -> None:
        self.config = config
        self.logger = logging.getLogger(self.config.name + "/client")
        self.channel = None
        self.stub = None

    def try_connect(self):
        if self.stub is not None:
            return True
        try:
            # create an async connection with the server
            self.channel = grpc.aio.insecure_channel(self.server_address)
            self.stub = event_service_pb2_grpc.EventServiceStub(self.channel)
        except Exception as exc:
            self.logger.warning("Could not connect to %s: %s", self.server_address, exc)
            return False

        return True

    @property
    def server_address(self) -> str:
        """Returns the composed address and port."""
        return f"{self.config.host}:{self.config.port}"

    async def get_state(self) -> ServiceState:
        state: ServiceState
        if not self.try_connect():
            return state

        try:
            response: GetServiceStateReply = await self.stub.getServiceState(
                GetServiceStateRequest()
            )
            state = ServiceState(response.state)
        except grpc.RpcError:
            state = ServiceState()
            self.stub = None
            self.channel = None
        self.logger.debug(
            f" {self.config.name} on port: %s state is: %s",
            self.config.port,
            state.name,
        )
        return state

    async def subscribe(self, request: SubscribeRequest, decode=True):
        response_stream = None
        message_cls = None

        while True:
            # check the state of the service
            state = await self.get_state()

            if state.value not in [ServiceStatePb.IDLE, ServiceStatePb.RUNNING]:
                if response_stream is not None:
                    response_stream.cancel()
                    response_stream = None

                self.logger.warning(
                    f"{self.config} is not streaming or ready to stream"
                )
                await asyncio.sleep(1)
                continue

            if response_stream is None and state.value != ServiceStatePb.UNAVAILABLE:
                # get the streaming object
                response_stream = self.stub.subscribe(request)

            try:
                # try/except so app doesn't crash on killed service
                response: SubscribeReply = await response_stream.read()
                response.event.timestamps.append(
                    get_monotonic_now(StampSemantics.DRIVER_RECEIVE)
                )
                assert response and response != grpc.aio.EOF, "End of stream"
            except Exception as exc:
                self.logger.warning("%s", exc)
                response_stream.cancel()
                response_stream = None
                await asyncio.sleep(1)
                continue

            if decode and message_cls is None:
                name, package = _parse_protobuf_descriptor(response.event.uri)
                message_cls = getattr(importlib.import_module(package), name)

            if decode:
                message: Message = message_cls()
                message.ParseFromString(response.payload)

                yield response.event, message
            else:
                yield response.event, response.payload

    async def listUris(self):
        if not self.try_connect():
            self.logger.warning("Could not list uris: %s", self.server_address)
            return []
        try:
            return (await self.stub.listUris(ListUrisRequest())).uris
        except grpc.RpcError as err:
            self.logger.warning(
                "Could not list uris: %s\n err=%s", self.server_address, err
            )
            return []


async def test_subscribe(client: EventClient, uri: Uri):
    async for uri, message in client.subscribe(SubscribeRequest(uri=uri, every_n=2)):
        print(uri, message)


async def test_get_state():
    config = EventServiceConfig(name="test_service", host="localhost", port=5001)
    client = EventClient(config)
    while (await client.get_state()).value not in [
        ServiceStatePb.IDLE,
        ServiceStatePb.RUNNING,
    ]:
        await asyncio.sleep(1)
        print("waiting for service to be ready")

    async_subscriptions: list[asyncio.Task] = []

    uris = []
    while True:
        uris = await client.listUris()
        if len(uris) > 0:
            break

    for uri in uris:
        async_subscriptions.append(asyncio.create_task(test_subscribe(client, uri)))
    await asyncio.gather(*async_subscriptions)


if __name__ == "__main__":

    asyncio.run(test_get_state())
