use crate::graphs::packets::PlottingPackets;
use crate::plotter_gui::PlotterGuiState;
use async_trait::async_trait;
use hollywood::compute::pipeline::CancelRequest;
use hollywood::core::actor::{ActorNode, DormantActorNode};
use hollywood::core::inbound::ForwardMessage;
use hollywood::core::runner::Runner;
use hollywood::core::*;
use hollywood::macros::actor_inputs;
use tokio::select;

#[derive(Clone, Debug)]
#[derive(Default)]
pub struct PlotterProp {}



/// Inbound message for the Plotter actor.
#[derive(Clone, Debug)]
#[actor_inputs(PlotterInbound, {PlotterProp, PlotterState, NullOutbound})]
pub enum PlotterMessage {
    Packets(PlottingPackets),
}

impl Value for PlotterProp {}

/// State of the actor.
#[derive(Clone, Debug)]
pub struct PlotterState {
    sender: Option<std::sync::mpsc::Sender<PlottingPackets>>,
}

impl PlotterState {
    pub fn new(sender: std::sync::mpsc::Sender<PlottingPackets>) -> Self {
        Self {
            sender: Some(sender),
        }
    }
}

impl Value for PlotterState {}

impl OnMessage for PlotterMessage {
    fn on_message(&self, _prop: &Self::Prop, _state: &mut Self::State, _outbound: &NullOutbound) {
        panic!("PlotterMessage::on_message() should never be called")
    }
}

impl InboundMessageNew<PlottingPackets> for PlotterMessage {
    fn new(_inbound_name: String, p: PlottingPackets) -> Self {
        PlotterMessage::Packets(p)
    }
}

pub struct PlotterActor {
    /// unique identifier of the actor
    pub actor_name: String,
    /// a collection of inbound channels
    pub inbound: PlotterInbound,
}

impl
    FromPropState<
        PlotterProp,
        PlotterInbound,
        PlotterState,
        NullOutbound,
        PlotterMessage,
        PlotterRunner,
    > for PlotterActor
{
    fn name_hint(_prop: &PlotterProp) -> String {
        "Plotter".to_owned()
    }
}

/// The custom runner for the plotter actor.
pub struct PlotterRunner {}

impl Runner<PlotterProp, PlotterInbound, PlotterState, NullOutbound, PlotterMessage>
    for PlotterRunner
{
    /// Create a new dormant actor.
    fn new_dormant_actor(
        name: String,
        prop: PlotterProp,
        state: PlotterState,
        receiver: tokio::sync::mpsc::Receiver<PlotterMessage>,
        _forward: std::collections::HashMap<
            String,
            Box<
                dyn ForwardMessage<PlotterProp, PlotterState, NullOutbound, PlotterMessage>
                    + Send
                    + Sync,
            >,
        >,
        _outbound: NullOutbound,
    ) -> Box<dyn DormantActorNode + Send + Sync> {
        Box::new(DormantPlotter {
            name: name.clone(),
            prop,
            state,
            receiver,
        })
    }
}

/// The dormant plotter actor.
pub struct DormantPlotter {
    name: String,
    prop: PlotterProp,
    state: PlotterState,
    pub(crate) receiver: tokio::sync::mpsc::Receiver<PlotterMessage>,
}

impl DormantActorNode for DormantPlotter {
    fn activate(self: Box<Self>) -> Box<dyn ActorNode + Send> {
        Box::new(ActivePlotterNodeImpl {
            name: self.name.clone(),
            _prop: self.prop.clone(),
            state: self.state,
            receiver: Some(self.receiver),
        })
    }
}

pub(crate) struct ActivePlotterNodeImpl {
    pub(crate) name: String,
    pub(crate) _prop: PlotterProp,
    pub(crate) state: PlotterState,
    pub(crate) receiver: Option<tokio::sync::mpsc::Receiver<PlotterMessage>>,
}

#[async_trait]
impl ActorNode for ActivePlotterNodeImpl {
    fn name(&self) -> &String {
        &self.name
    }

    fn reset(&mut self) {
        // no-op
    }

    async fn run(&mut self, mut kill: tokio::sync::broadcast::Receiver<()>) {
        let sender = self.state.sender.take().unwrap();

        let mut receiver = self.receiver.take().unwrap();

        let on_plotter_message = |m: PlotterMessage| match m {
            PlotterMessage::Packets(packets) => match sender.send(packets) {
                Ok(_) => {}
                Err(w) => {
                    println!("{}", w)
                }
            },
        };

        loop {
            select! {
                _ = kill.recv() => {

                    while receiver.try_recv().is_ok(){}

                    return;
                },
                m = receiver.recv() => {
                    if m.is_none() {
                        let _ = kill.try_recv();
                        return;
                    }
                   on_plotter_message(m.unwrap());
                }
            }
        }
    }
}

pub fn run_on_main_thread(
    receiver: std::sync::mpsc::Receiver<PlottingPackets>,
    cancel_request_sender: tokio::sync::mpsc::Sender<CancelRequest>,
) {
    let gui = PlotterGuiState::new(receiver, cancel_request_sender);
    let options = eframe::NativeOptions::default();
    eframe::run_native(
        "Remote Plotting Service",
        options,
        Box::new(|_cc| Box::new(gui)),
    )
    .unwrap();
}
