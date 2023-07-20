from __future__ import annotations

# FIXME simplify this code as much as possible.

import asyncio
from asyncio.subprocess import Process
from enum import Enum
from threading import Lock
import psutil
from datetime import datetime
import logging

DATETIME_FORMAT: str = "%Y_%m_%d_%H_%M_%S_%f"


def current_time_now() -> str:
    """Get the current time as a string.

    Returns:
        str: The current time as a string.
    """
    dt = datetime.now()  # current time
    dt_str = dt.strftime(DATETIME_FORMAT)
    return dt_str


class SubprocessState(Enum):
    """State of a subprocess."""

    UNKNOWN = 0
    RUNNING = 1
    STOPPED = 2


class Subprocess:
    """Defines a subprocess to be managed by the SubprocessManager."""

    def __init__(
        self, name: str, args: list[str], logger: logging.Logger = None
    ) -> None:
        """Initialize the Subprocess.

        Args:
            name (str): Name of the subprocess.
            args (list[str]): List of arguments to pass to the subprocess.
        """
        self.name = name  # name of the subprocess
        self._args: list[str] = args  # list of arguments to pass to the subprocess
        self.process: Process | None = None  # asyncio subprocess
        self.monitor_task: asyncio.Task | None = (
            None  # asyncio task to monitor the subprocess logs
        )

        # add this subprocess to the manager
        get_subprocess_manager().add(self)

        self.logger = logger
        if self.logger is None:
            self.logger = logging.getLogger(name)

    @property
    def pid(self) -> int:
        """Return the PID of the subprocess.

        Returns:
            int: PID of the subprocess.
        """
        if self.process is None:
            return -1
        return self.process.pid

    @property
    def state(self) -> SubprocessState:
        """Return the state of the subprocess.

        Returns:
            SubprocessState: State of the subprocess.
        """
        if self.process is None:
            return SubprocessState.UNKNOWN
        if self.process.returncode is None:
            return SubprocessState.RUNNING
        return SubprocessState.STOPPED

    @property
    def args(self) -> list[str]:
        """Return the list of arguments to pass to the subprocess.

        Returns:
            list[str]: List of arguments to pass to the subprocess.
        """
        return self._args

    @args.setter
    def args(self, args: list[str]):
        """Update the list of all arguments (including executable) to pass to the subprocess.
        Args:
            args (list[str]): List of arguments to pass to the subprocess.
        """
        self._args = args

    async def start(self) -> bool:
        """Start the subprocess.

        Returns:
            bool: True if the subprocess was started successfully, False otherwise.
        """
        if self.process is not None and self.state == SubprocessState.RUNNING:
            self.logger.debug("Subprocess already running")
            return False

        try:
            self.logger.info(
                f"Starting subprocess {self.name} with command:\n {self.args}"
            )

            # create the subprocess
            self.process = await asyncio.create_subprocess_exec(
                *self.args,
                stdout=asyncio.subprocess.PIPE,
                stderr=asyncio.subprocess.PIPE,
            )
            # Start monitoring the subprocess, asynchronously so we can return from this function
            self.monitor_task = asyncio.create_task(self._start_monitoring())
        except Exception as e:
            self.logger.exception(f"Failed to start subprocess {self.name}: {e}")
            return False

        self.logger.info(f"Started subprocess {self.name} with PID {self.pid}")
        return True

    async def _terminate_child_processes(self) -> None:
        # get the parent process to terminate
        try:
            parent = psutil.Process(self.pid)
        except psutil.NoSuchProcess:
            self.logger.exception(
                f"Subprocess {self.name} with PID {self.pid} not found"
            )
            return

        # get the children processes (if any)
        children = parent.children(recursive=True)

        try:
            self.process.terminate()  # terminate the parent process
        except Exception as e:
            self.logger.exception(f"Failed to terminate subprocess {self.name}: {e}")
            return
        await self.process.wait()  # wait for the parent process to terminate

        if self.state != SubprocessState.STOPPED:
            self.logger.error(
                f"Failed to terminate subprocess {self.name} with PID {self.pid}"
            )

        # terminate the children processes if still running
        for process in children:
            try:
                process.terminate()
            except Exception as e:
                self.logger.exception(
                    f"Failed to terminate subprocess {self.name}: {e}"
                )
                return

    async def stop(self) -> bool:
        """Stop the subprocess.

        Returns:
            bool: True if the subprocess was stopped successfully, False otherwise.
        """
        if self.state == SubprocessState.UNKNOWN:
            self.logger.debug("Subprocess not started")
            return False
        elif self.state == SubprocessState.STOPPED:
            self.logger.debug("Subprocess already stopped")
            # check if the monitoring task is still running because
            # we may have missed the event that the subprocess stopped
            if not self.monitor_task.cancelled():
                self.monitor_task.cancel()
            return False

        try:
            self.monitor_task.cancel()  # cancel the monitoring tasks
            await self._terminate_child_processes()  # terminate the subprocess and its children
        except Exception as e:
            self.logger.exception(f"Failed to stop subprocess {self.name}: {e}")
            return False

        self.logger.info(f"Stopped subprocess {self.name} with PID {self.pid}")
        return True

    async def _start_monitoring(self) -> None:
        """Start monitoring the subprocess logs."""
        # format and open the log file
        start_time: str = current_time_now()
        self.logger.info(f"Starting {self.name} at {start_time} with PID {self.pid}")

        # start monitoring the subprocess in a background task
        await asyncio.gather(
            self._io_stream(self.process.stdout),
            self._io_stream(self.process.stderr),
        )

        end_time: str = current_time_now()
        self.logger.info(
            f"Stopping {self.name} at {end_time} with PID {self.pid} returncode {self.process.returncode}"
        )

    # TODO: during tests->RuntimeWarning: coroutine 'Subprocess._io_stream' was never awaited
    async def _io_stream(self, stream: asyncio.StreamReader) -> None:
        """Monitor the subprocess logs."""
        while True:
            line_bytes: bytes = await stream.readline()
            if not line_bytes:
                return
            line: str = line_bytes.strip().decode("utf-8")
            self.logger.info(f"[{self.pid}] {line}")


# The Singleton class defines the `instance` method that lets clients access
# https://refactoring.guru/design-patterns/singleton/python/example


class SingletonMeta(type):
    """
    This is a thread-safe implementation of Singleton.
    """

    _instances = {}

    _lock: Lock = Lock()
    """
    We now have a lock object that will be used to synchronize threads during
    first access to the Singleton.
    """

    def __call__(cls, *args, **kwargs):
        """
        Possible changes to the value of the `__init__` argument do not affect
        the returned instance.
        """
        # Now, imagine that the program has just been launched. Since there's no
        # Singleton instance yet, multiple threads can simultaneously pass the
        # previous conditional and reach this point almost at the same time. The
        # first of them will acquire lock and will proceed further, while the
        # rest will wait here.
        with cls._lock:
            # The first thread to acquire the lock, reaches this conditional,
            # goes inside and creates the Singleton instance. Once it leaves the
            # lock block, a thread that might have been waiting for the lock
            # release may then enter this section. But since the Singleton field
            # is already initialized, the thread won't create a new object.
            if cls not in cls._instances:
                instance = super().__call__(*args, **kwargs)
                cls._instances[cls] = instance
        return cls._instances[cls]


class SubprocessManager(metaclass=SingletonMeta):
    """Manages a set of subprocesses."""

    def __init__(self) -> None:
        """Initialize the SubprocessManager."""
        self.processes: set[Subprocess] = set()

    def add(self, subprocess: Subprocess) -> None:
        """Add a subprocess to the manager.

        Args:
            subprocess (Subprocess): The subprocess to add.
        """
        if not isinstance(subprocess, Subprocess):
            raise ValueError("Subprocess must be of type Subprocess")
        if subprocess in self.processes:
            raise ValueError(f"Subprocess {subprocess.name} already exists")
        self.processes.add(subprocess)

    def remove(self, subprocess: Subprocess) -> None:
        """Remove a subprocess from the manager.

        Args:
            subprocess (Subprocess): The subprocess to remove.
        """
        if not isinstance(subprocess, Subprocess):
            raise ValueError("Subprocess must be of type Subprocess")
        if subprocess not in self.processes:
            raise ValueError(f"Subprocess {subprocess.name} does not exist")
        self.processes.remove(subprocess)

    def get(self, name: str) -> Subprocess | None:
        """Get a subprocess by name."""
        for process in self.processes:
            if process.name == name:
                return process
        return None


# A singleton instance of the SubprocessManager
_subprocess_manager = SubprocessManager()


def get_subprocess_manager() -> SubprocessManager:
    """Get the singleton instance of the SubprocessManager."""
    return _subprocess_manager
