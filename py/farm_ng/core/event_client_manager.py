"""Module for the EventServiceBackend class."""
from __future__ import annotations

import asyncio
import logging

from farm_ng.core.event_client import EventClient
from farm_ng.core.event_service_pb2 import (
    EventServiceConfig,
    EventServiceConfigList,
    SubscribeRequest,
)
from farm_ng.core.uri_pb2 import Uri

__all__ = ["EventClientSubscriptionManager"]


class EventClientSubscriptionManager:
    """Class for the event client subscription manager.

    This class is responsible for managing the subscriptions for the service
    and maintaining the list of clients.

    Attributes:
        clients: A map of service name to EventClient.
        subscriptions: A map of service_path to SubscribeRequest.
    """

    def __init__(
        self,
        config_list: EventServiceConfigList,
        logger: logging.Logger | None = None,
    ) -> None:
        """Initializes the EventServiceBackend.

        Args:
            config_list: The configuration list for the service.
            logger: The logger for the service.
        """
        self._config_list = config_list

        # initialize the logger, ideally we would like to use the same logger as the
        # service, but we can't do that because the service is not initialized yet.
        self._logger = logger or logging.getLogger()

        # initialize a map of service name to EventClient
        self._clients: dict[str, EventClient] = self._initialize_clients(config_list)

        # initialize a map of service_path to SubscribeRequest
        self._subscriptions: dict[str, SubscribeRequest] = {}

    @property
    def clients(self) -> dict[str, EventClient]:
        """Returns a map of service name to EventClient."""
        return self._clients

    @property
    def logger(self) -> logging.Logger:
        """Returns the logger for the service."""
        return self._logger

    def _initialize_clients(
        self,
        config_list: EventServiceConfigList,
    ) -> dict[str, list[SubscribeRequest]]:
        """Initializes the clients associated with the service.

        Creates a map of service name to EventClient.

        Args:
            config_list: The configuration list for the service.

        Returns:
            A map of service name to EventClient.
        """
        # create a map of service name to EventClient
        # key: service name, value: EventClient
        clients: dict[str, EventClient] = {}

        config: EventServiceConfig
        for config in config_list.configs:
            if config.port is None:
                msg = "Port is not set for service"
                raise ValueError(msg)
            # update the service config
            clients.update({config.name: EventClient(config)})
        return clients

    def _try_register_topic(self, service_name: str, uri_path: str) -> bool:
        """Attempts to register a topic for a service.

        Args:
            service_name: The name of the service.
            uri_path: The path of the topic.

        Returns:
            True if the topic was registered successfully, False otherwise.
        """
        if service_name not in self._clients:
            self.logger.warning("Service %s not found", service_name)
            return False

        # make the key for the subscription map
        service_path: str = f"{service_name}{uri_path}"

        if service_path in self._subscriptions:
            # self.logger.warning("Topic %s already registered", service_path)
            return False

        subscribe_request = SubscribeRequest(
            uri=Uri(
                path=uri_path,
                query=f"service_name={service_name}",
            ),
            every_n=1,
        )

        self.logger.info("Registering topic %s", service_path)
        self._subscriptions[service_path] = subscribe_request

        return True

    async def update_subscriptions_for_client(self, client: EventClient) -> None:
        """Updates the subscriptions for a client.

        Args:
            client: The client to update the subscriptions for.
        """
        while True:
            # get the list of uris from the client
            uris: list[Uri] = await client.list_uris()

            # register the topics
            uri: Uri
            for uri in uris:
                self._try_register_topic(client.config.name, uri.path)

            await asyncio.sleep(1.0)

    async def update_subscriptions(self) -> None:
        """Serves the subscriptions once to create the topics in foxglove."""
        tasks: list[asyncio.Task] = []

        client: EventClient
        for client in self._clients.values():
            tasks.append(
                asyncio.create_task(self.update_subscriptions_for_client(client)),
            )
        await asyncio.gather(*tasks)

    def get_all_uris_config_list(self, config_name: str) -> EventServiceConfigList:
        """Returns a config list with all the subscriptions for the service.

        Args:
            config_name: The name of the generated config containing all the subscriptions.

        Returns:
            A config list with all the subscriptions for the service.
        """
        # make a copy of the config list
        config_list = EventServiceConfigList()
        config_list.CopyFrom(self._config_list)

        # add the default service config and append the subscriptions
        service_config = EventServiceConfig(name=config_name)

        subscription: SubscribeRequest
        for subscription in self._subscriptions.values():
            service_config.subscriptions.append(subscription)

        config_list.configs.append(service_config)

        return config_list
