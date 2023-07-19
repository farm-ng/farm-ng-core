from dataclasses import dataclass
import asyncio


from typing import AsyncIterator
import logging
import grpc
from farm_ng.core import event_service_pb2_grpc
from farm_ng.core import event_service_pb2
import time
import os


@dataclass
class ServiceGrpcConfiguration:
    """Configuration for a service."""

    name: str
    port: int
    host: str


class EventServiceGrpc:
    """Base class for a gRPC service."""

    def __init__(
        self,
        server: grpc.aio.Server,
        config: ServiceGrpcConfiguration,
        logger: logging.Logger = None,
    ) -> None:
        """Initializes a new service.

        Args:
            server: The gRPC server instance.
            config: The service configuration.
        """
        self._server = server
        self._config = config

        # create a logger for the service
        self._logger: logging.Logger
        if logger is not None:
            self._logger = logger
        else:
            self._logger = logging.getLogger(config.name)

        # the initial state when doing nothing must be IDLE
        self.state: event_service_pb2.ServiceState = event_service_pb2.ServiceState.IDLE

        # the time when the service was started
        self.time_started: float = time.monotonic()

        # add the service to the server
        event_service_pb2_grpc.add_EventServiceServicer_to_server(self, server)

    @property
    def server(self) -> grpc.aio.Server:
        """Returns the server instance."""
        return self._server

    @property
    def config(self) -> ServiceGrpcConfiguration:
        """Returns the service configuration."""
        return self._config

    @property
    def logger(self) -> logging.Logger:
        """Returns the service logger."""
        return self._logger

    async def serve(self) -> None:
        """Starts the service.

        This method is called by the service manager.
        """
        self.server.add_insecure_port(f"[::]:{self.config.port}")

        self.logger.info("Starting server on port %i", self.config.port)
        await self.server.start()

        self.logger.info("Server started")
        await self.server.wait_for_termination()

    def getServiceState(
        self,
        request: event_service_pb2.GetServiceStateRequest,
        context: grpc.aio.ServicerContext,
    ) -> event_service_pb2.GetServiceStateReply:
        """Get the service state.

        Args:
            request (service_pb2.GetServiceStateRequest): The request.
            context (grpc.aio.ServicerContext): The context.

        Returns:
            service_pb2.GetServiceStateReply: The reply.
        """
        return event_service_pb2.GetServiceStateReply(
            state=self.state,
            pid=os.getpid(),
            uptime=time.monotonic() - self.time_started,
            timestamps=[],  # TODO
        )

    async def listUris(
        self,
        request: event_service_pb2.ListUrisRequest,
        context: grpc.aio.ServicerContext,
    ) -> event_service_pb2.ListUrisReply:
        """List the URIs of the service.

        Args:
            request (service_pb2.ListUrisRequest): The request.
            context (grpc.aio.ServicerContext): The context.

        Returns:
            service_pb2.ListUrisReply: The reply.
        """
        return event_service_pb2.ListUrisReply(uris=[])
    
    async def publish(self, request: event_service_pb2.PublishRequest, context: grpc.aio.ServicerContext) -> event_service_pb2.PublishReply:
        """Publish an event.

        Args:
            request (service_pb2.PublishRequest): The request.
            context (grpc.aio.ServicerContext): The context.

        Returns:
            service_pb2.PublishReply: The reply.
        """
        return event_service_pb2.PublishReply()
    
    async def subscribe(
        self,
        request: event_service_pb2.SubscribeRequest,
        context: grpc.aio.ServicerContext,
    ) -> AsyncIterator[event_service_pb2.SubscribeReply]:

        if self.state != event_service_pb2.ServiceState.RUNNING:
            self.logger.info(f"Setting {self.__class__.__name__} state to running")
            self.state = event_service_pb2.ServiceState.RUNNING

        # create a queue for this client
        queue = asyncio.Queue(maxsize=10)
        self.listener.client_queues_rawcan.append(queue)

        def delete_queue(_) -> None:
            # remove the queue from the list of queues
            self.listener.client_queues_rawcan.remove(queue)

            # check that there are no more clients connected
            if self.listener.num_clients() == 0:
                self.logger.info(f"Setting {self.__class__.__name__} to idle state")
                self.state = event_service_pb2.ServiceState.IDLE

        context.add_done_callback(delete_queue)

        while True:
            yield await queue.get()


# main function to run the service and all the async tasks
async def main(event_service: EventServiceGrpc) -> None:
    # define the async tasks
    async_tasks: list[asyncio.Task] = []
    async_tasks.append(event_service.serve())

    # run the tasks
    await asyncio.gather(*async_tasks)


if __name__ == "__main__":

    service_config = ServiceGrpcConfiguration(
        name="test_service",
        port=5001,
        host="localhost",
    )

    # create the gRPC server and initialize the service
    server = grpc.aio.server()
    event_service: EventServiceGrpc = EventServiceGrpc(server, service_config)

    loop = asyncio.get_event_loop()
    loop.run_until_complete(main(event_service=event_service))
