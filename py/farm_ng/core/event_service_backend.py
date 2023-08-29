"""Module for the EventServiceBackend class."""
from __future__ import annotations

import asyncio

from farm_ng.core.event_client import EventClient
from farm_ng.core.event_service_pb2 import (
    EventServiceConfig,
    EventServiceConfigList,
    SubscribeRequest,
)
from farm_ng.core.uri_pb2 import Uri


class EventServiceBackend:
    """Class for the event service backend.

    This class is responsible for managing the subscriptions for the service
    and maintaining the list of clients.

    Attributes:
        service_config: The configuration for the service.
        clients: A map of service name to EventClient.
        subscriptions: A map of service_path to SubscribeRequest.
    """

    def __init__(
        self,
        service_config: EventServiceConfig,
        config_list: EventServiceConfigList,
    ) -> None:
        """Initializes the EventServiceBackend.

        Args:
            service_config: The configuration for the service.
            config_list: The configuration list for the service.
        """
        self._service_config: EventServiceConfig = service_config

        # initialize a map of service name to EventClient
        self._clients: dict[str, EventClient] = self._initialize_clients(config_list)

        # initialize a map of service_path to SubscribeRequest
        self._subscriptions: dict[str, SubscribeRequest] = {}

    @property
    def service_config(self) -> EventServiceConfig:
        """Returns the configuration for the service."""
        return self._service_config

    @property
    def clients(self) -> dict[str, EventClient]:
        """Returns a map of service name to EventClient."""
        return self._clients

    @property
    def subscriptions(self) -> dict[str, SubscribeRequest]:
        """Returns a map of service_path to SubscribeRequest."""
        return self._subscriptions

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
            if config.port:
                # the config name is the service name
                clients[config.name] = EventClient(config)
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
            print(f"Service {service_name} not found")
            return False

        # make the key for the subscription map
        service_path: str = f"{service_name}{uri_path}"

        if service_path in self._subscriptions:
            print(f"Topic {service_path} already registered")
            return False

        subscribe_request = SubscribeRequest(
            uri=Uri(
                path=uri_path,
                query=f"service_name={service_name}",
            ),
            every_n=1,
        )

        print(f"Registering subscription: {service_path}")
        self._subscriptions[service_path] = subscribe_request

        return True

    async def update_subscriptions_for_client(self, client: EventClient):
        while True:
            uris: list[Uri] = await client.list_uris()
            for uri in uris:
                await self._try_register_topic(client.config.name, uri.path)
            await asyncio.sleep(1.0)

    async def update_subscriptions(self) -> None:
        """Serves the subscriptions once to create the topics in foxglove."""
        tasks: list[asyncio.Task] = []

        client: EventClient
        for client in self._clients.values():
            # skip adding subscriptions for the service itself
            if client.config.name == self._service_config.name:
                continue

            tasks.append(
                asyncio.create_task(self.update_subscriptions_for_client(client)),
            )
        await asyncio.gather(*tasks)

    def get_all_uris_config_list(self) -> EventServiceConfigList:
        """Returns a config list with all the subscriptions for the service.

        Returns:
            A config list with all the subscriptions for the service.
        """
        config_list = EventServiceConfigList()
        client: EventClient
        for client in self._clients.values():
            if client.config.name == self._service_config.name:
                continue
            config_list.configs.append(client.config)

        # add the default service config
        service_config: EventServiceConfig = EventServiceConfig()
        service_config.name = f"{self._service_config.name}_default"

        # append the subscriptions
        for subscription in self._subscriptions.values():
            service_config.subscriptions.append(subscription)
        config_list.configs.append(service_config)

        return config_list
