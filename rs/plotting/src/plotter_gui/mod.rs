use std::ops::RangeInclusive;

use eframe::egui;

use hollywood::compute::pipeline::CancelRequest;

use crate::graphs::common::Bounds;
use crate::graphs::packets::{PlottingPacket, PlottingPackets};
use crate::graphs::scalar_curve::ScalarCurve;
use crate::graphs::vec3_curve::Vec3Curve;

/// This is the gui app of the plotting service.
pub struct PlotterGuiState {
    pub receiver: std::sync::mpsc::Receiver<PlottingPackets>,
    pub cancel_requester: tokio::sync::mpsc::Sender<CancelRequest>,

    /// holds a collection of plots indexed by plot_name
    pub plots: std::collections::BTreeMap<String, Plot>,
    selected_plot: String,

    first_plot_received: bool,
}

impl From<crate::graphs::common::Color> for egui::Color32 {
    fn from(val: crate::graphs::common::Color) -> Self {
        egui::Color32::from_rgba_premultiplied(
            (val.r * 255.0) as u8,
            (val.g * 255.0) as u8,
            (val.b * 255.0) as u8,
            (val.a * 255.0) as u8,
        )
    }
}

#[derive(Clone, Debug)]
pub enum GraphType {
    Scalar(ScalarCurve),
    Vec3(Vec3Curve),
}

#[derive(Clone, Debug)]
pub struct CurveStruct {
    pub curve: GraphType,
    pub show_graph: bool,
}

/// a single plot is a collection of curves, indexed by curve_name
#[derive(Clone, Debug)]
pub struct Plot {
    pub curves: std::collections::BTreeMap<String, CurveStruct>,
    pub bounds: Bounds,
    pub show_plot: bool,
    pub mouse_nav: bool,
    pub show_axis: [bool; 3],
}

impl PlotterGuiState {
    pub fn new(
        receiver: std::sync::mpsc::Receiver<PlottingPackets>,
        cancel_requester: tokio::sync::mpsc::Sender<CancelRequest>,
    ) -> Self {
        PlotterGuiState {
            receiver,
            cancel_requester,
            plots: std::collections::BTreeMap::new(),
            selected_plot: String::new(),
            first_plot_received: false,
        }
    }
}

impl eframe::App for PlotterGuiState {
    fn on_exit(&mut self, _gl: Option<&eframe::glow::Context>) {
        self.cancel_requester
            .try_send(CancelRequest::Cancel(()))
            .unwrap();
    }

    /// Called by eframe to update the app
    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        for msg_vec in self.receiver.try_iter() {
            for msg in msg_vec {
                match msg {
                    PlottingPacket::ScalarCurve(new_value) => {
                        let plot_name = new_value.plot_name.clone();
                        let curve_name = new_value.graph_name.clone();

                        let plot = self.plots.entry(plot_name.clone()).or_insert(Plot {
                            curves: std::collections::BTreeMap::new(),
                            bounds: new_value.scalar_curve.bounds,
                            show_plot: !self.first_plot_received,
                            mouse_nav: false,
                            show_axis: [true, true, true],
                        });
                        self.first_plot_received = true;
                        plot.curves
                            .entry(curve_name.clone())
                            .and_modify(|curve_struct| match &mut curve_struct.curve {
                                GraphType::Scalar(g) => {
                                    g.append(
                                        new_value.scalar_curve.data.clone(),
                                        new_value.scalar_curve.color,
                                        new_value.scalar_curve.curve_type.clone(),
                                        new_value.scalar_curve.clear_x_smaller_than.clone(),
                                    );
                                }
                                GraphType::Vec3(_g) => {}
                            })
                            .or_insert(CurveStruct {
                                curve: GraphType::Scalar(new_value.scalar_curve.clone()),
                                show_graph: true,
                            });
                    }

                    PlottingPacket::Vec3Curve(new_value) => {
                        let plot_name = new_value.plot_name.clone();
                        let curve_name = new_value.graph_name.clone();

                        let plot = self.plots.entry(plot_name.clone()).or_insert(Plot {
                            curves: std::collections::BTreeMap::new(),
                            bounds: new_value.scalar_curve.bounds,
                            show_plot: !self.first_plot_received,
                            mouse_nav: false,
                            show_axis: [true, true, true],
                        });
                        self.first_plot_received = true;
                        plot.curves
                            .entry(curve_name.clone())
                            .and_modify(|curve_struct| match &mut curve_struct.curve {
                                GraphType::Scalar(_s) => {}
                                GraphType::Vec3(g) => {
                                    g.append(
                                        new_value.scalar_curve.data.clone(),
                                        new_value.scalar_curve.color,
                                        new_value.scalar_curve.curve_type.clone(),
                                        new_value.scalar_curve.clear_x_smaller_than.clone(),
                                    );
                                }
                            })
                            .or_insert(CurveStruct {
                                curve: GraphType::Vec3(new_value.scalar_curve.clone()),
                                show_graph: true,
                            });
                    }
                }
            }
        }

        egui::SidePanel::left("toggles").show(ctx, |ui| {
            {
                for (plot_name, plot) in &mut self.plots {
                    ui.horizontal(|ui| {
                        ui.checkbox(&mut plot.show_plot, "");
                        if ui
                            .add(egui::SelectableLabel::new(
                                plot_name == &self.selected_plot,
                                plot_name.clone(),
                            ))
                            .clicked()
                        {
                            self.selected_plot = plot_name.clone();
                        }
                    });
                }
            }

            ui.separator();

            for (plot_name, plot) in &mut self.plots {
                if &self.selected_plot != plot_name {
                    continue;
                }
                ui.label(format!("Graphs in '{}' plot", plot_name));

                for (curve_name, curve) in &mut plot.curves {
                    ui.checkbox(&mut curve.show_graph, curve_name);
                }

                ui.separator();
                ui.label(format!("Bounds of '{}' plot", plot_name));

                ui.add_enabled_ui(!plot.mouse_nav, |ui| {
                    ui.horizontal_wrapped(|ui| {
                        ui.label("width");
                        ui.add(
                            egui::DragValue::new(&mut plot.bounds.x_bounds.len)
                                .clamp_range(RangeInclusive::new(0.0, 10000.0)),
                        );
                    });
                    ui.horizontal_wrapped(|ui| {
                        ui.label("height");
                        ui.add(
                            egui::DragValue::new(&mut plot.bounds.y_bounds.len)
                                .clamp_range(RangeInclusive::new(0.0, 10000.0)),
                        );
                    });

                    ui.horizontal_wrapped(|ui| {
                        ui.add_enabled_ui(!plot.bounds.x_bounds.data_driven, |ui| {
                            ui.label("x-max");
                            ui.add(egui::DragValue::new(&mut plot.bounds.x_bounds.largest));
                        });
                        ui.checkbox(&mut plot.bounds.x_bounds.data_driven, "data-driven");
                    });

                    ui.horizontal_wrapped(|ui| {
                        ui.add_enabled_ui(!plot.bounds.y_bounds.data_driven, |ui| {
                            ui.label("y-max");
                            ui.add(egui::DragValue::new(&mut plot.bounds.y_bounds.largest));
                        });
                        ui.checkbox(&mut plot.bounds.y_bounds.data_driven, "data-driven");
                    });
                });
                ui.checkbox(&mut plot.mouse_nav, "mouse nav");

                ui.separator();

                // Toggle axes
                {
                    for i in 0..3 {
                        let axis = &mut plot.show_axis[i];
                        ui.toggle_value(axis, format!("show axis-{}", i));
                    }
                }
            }
            ui.separator();

            if ui.add(egui::Button::new("reset")).clicked() {
                self.plots = std::collections::BTreeMap::new();
            }
        });

        egui::CentralPanel::default().show(ctx, |ui| {
            // 1. Get the available height
            let h = ui.available_height();

            let num_checked = self.plots.values().filter(|x| x.show_plot).count();

            // 2.Calculate the height per plot
            let height_per_plot = (h / num_checked as f32).floor();

            // 3. Plot each plot to the ui

            for (plot_name, plot_data) in &mut self.plots {
                if !plot_data.show_plot {
                    continue;
                }
                egui_plot::Plot::new(plot_name)
                    .height(height_per_plot)
                    .legend(egui_plot::Legend::default().position(egui_plot::Corner::LeftTop))
                    .auto_bounds_x()
                    .auto_bounds_y()
                    .show(ui, |plot_ui| {
                        let mut data_driven_largest_x = -std::f64::INFINITY;
                        let mut data_driven_largest_y = -std::f64::INFINITY;

                        if plot_ui.response().double_clicked() {
                            plot_data.mouse_nav = false;
                            self.selected_plot = plot_name.clone();
                        } else if plot_ui.response().clicked() || plot_ui.response().dragged() {
                            plot_data.mouse_nav = true;
                            self.selected_plot = plot_name.clone();
                        }

                        for (curve_name, graph_data) in &mut plot_data.curves {
                            if !graph_data.show_graph {
                                continue;
                            }

                            match &graph_data.curve {
                                GraphType::Scalar(g) => {
                                    let mut points = vec![];

                                    for (x, y) in &g.data {
                                        if x > &data_driven_largest_x {
                                            data_driven_largest_x = *x;
                                        }

                                        if y > &data_driven_largest_y {
                                            data_driven_largest_y = *y;
                                        }

                                        points.push(egui_plot::PlotPoint::new(*x, *y));
                                    }
                                    let plot_points = egui_plot::PlotPoints::Owned(points);

                                    match g.curve_type {
                                        crate::graphs::common::LineType::LineStrip => {
                                            plot_ui.line(
                                                egui_plot::Line::new(plot_points)
                                                    .color(g.color)
                                                    .name(curve_name),
                                            );
                                        }
                                        crate::graphs::common::LineType::Points => {
                                            plot_ui.points(
                                                egui_plot::Points::new(plot_points)
                                                    .color(g.color)
                                                    .name(curve_name),
                                            );
                                        }
                                    }
                                }
                                GraphType::Vec3(g) => {
                                    let mut points = vec![vec![], vec![], vec![]];

                                    for (x, y) in &g.data {
                                        if x > &data_driven_largest_x {
                                            data_driven_largest_x = *x;
                                        }

                                        let max_y = y.0.max(y.1.max(y.2));
                                        if max_y > data_driven_largest_y {
                                            data_driven_largest_y = max_y;
                                        }

                                        points[0].push(egui_plot::PlotPoint::new(*x, y.0));
                                        points[1].push(egui_plot::PlotPoint::new(*x, y.1));
                                        points[2].push(egui_plot::PlotPoint::new(*x, y.2));
                                    }

                                    match g.curve_type {
                                        crate::graphs::common::LineType::LineStrip => {
                                            for (i, p) in points.iter().enumerate().take(3) {
                                                if plot_data.show_axis[i] {
                                                    let plot_points =
                                                        egui_plot::PlotPoints::Owned(p.clone());
                                                    plot_ui.line(
                                                        egui_plot::Line::new(plot_points)
                                                            .color(g.color[i])
                                                            .name(format!("{}-{}", curve_name, i)),
                                                    );
                                                }
                                            }
                                        }
                                        crate::graphs::common::LineType::Points => {
                                            for (i, p) in points.iter().enumerate().take(3) {
                                                if plot_data.show_axis[i] {
                                                    let plot_points =
                                                        egui_plot::PlotPoints::Owned(p.clone());
                                                    plot_ui.line(
                                                        egui_plot::Line::new(plot_points)
                                                            .color(g.color[i])
                                                            .name(format!("{}-{}", curve_name, i)),
                                                    );
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        if !plot_data.mouse_nav {
                            let largest_x = if plot_data.bounds.x_bounds.data_driven {
                                data_driven_largest_x
                            } else {
                                plot_data.bounds.x_bounds.largest
                            };
                            let largest_y = if plot_data.bounds.y_bounds.data_driven {
                                data_driven_largest_y
                            } else {
                                plot_data.bounds.y_bounds.largest
                            };
                            plot_ui.set_plot_bounds(egui_plot::PlotBounds::from_min_max(
                                [
                                    largest_x - plot_data.bounds.x_bounds.len,
                                    largest_y - plot_data.bounds.y_bounds.len,
                                ],
                                [largest_x, largest_y],
                            ));
                            plot_data.bounds.x_bounds.largest = largest_x;
                            plot_data.bounds.y_bounds.largest = largest_y;
                        }
                    });
            }
        });

        ctx.request_repaint_after(std::time::Duration::from_secs_f64(0.01));
    }
}
