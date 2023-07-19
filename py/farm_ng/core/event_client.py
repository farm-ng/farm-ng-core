import logging
from dataclasses import dataclass
import grpc
from farm_ng.core import event_service_pb2_grpc
from farm_ng.core import event_service_pb2
import asyncio

logging.basicConfig(level=logging.INFO)


@dataclass
class ClientConfig:
    """Client configuration.

    Attributes:
        port (int): the port to connect to the server.
        address (str): the address to connect to the server.
    """

    port: int  # the port of the server address
    address: str = "localhost"  # the address name of the server


class ServiceState:
    """Generic service state.

    Possible state values:
        - UNKNOWN: undefined state.
        - RUNNING: the service is up AND streaming.
        - IDLE: the service is up AND NOT streaming.
        - UNAVAILABLE: the service is not available.
        - ERROR: the service is an error state.

    Args:
        proto (event_service_pb2.ServiceState): protobuf message containing the service state.
    """

    def __init__(self, proto: event_service_pb2.ServiceState = None) -> None:
        self._proto = event_service_pb2.ServiceState.UNAVAILABLE
        if proto is not None:
            self._proto = proto

    @property
    def value(self) -> int:
        """Returns the state enum value."""
        return self._proto

    @property
    def name(self) -> str:
        """Return the state name."""
        return event_service_pb2.ServiceState.DESCRIPTOR.values[self.value].name

    def __repr__(self) -> str:
        return f"{self.__class__.__name__}: ({self.value}, {self.name})"


class ServiceClient:
    """Generic client.

    Generic client class to connect with the Amiga brain services.
    Internally implements an `asyncio` gRPC channel.
    Designed to be imported by service specific clients.

    Args:
        config (ClientConfig): the grpc configuration data structure.
    """

    def __init__(self, config: ClientConfig) -> None:
        print('config in ServiceClient', config)
        self.config = config

        self.logger = logging.getLogger(self.__class__.__name__)

        # create an async connection with the server
        self.channel = grpc.aio.insecure_channel(self.server_address)
        self.state_stub = event_service_pb2_grpc.EventServiceStub(self.channel)

    @property
    def server_address(self) -> str:
        """Returns the composed address and port."""
        return f"{self.config.address}:{self.config.port}"

    async def get_state(self) -> ServiceState:
        state: ServiceState
        try:
            response: event_service_pb2.GetServiceStateReply = await self.state_stub.getServiceState(
                event_service_pb2.GetServiceStateRequest()
            )
            state = ServiceState(response.state)
        except grpc.RpcError:
            state = ServiceState()
        self.logger.debug(f" {self.__class__.__name__} on port: %s state is: %s", self.config.port, state.name)
        return state


    def subscribe(self, request: event_service_pb2.SubscribeRequest):
        return self.stub.subscribe(request)
    
    def publish(self, request: event_service_pb2.PublishRequest):
        return self.stub.publish(request)
    

async def test_get_state():
    config = ClientConfig(port=5001)
    client = ServiceClient(config)
    print(await client.get_state())


if __name__ == "__main__":
    
    asyncio.run(test_get_state())

    
    