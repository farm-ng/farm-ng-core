use eframe::egui;

use crate::{actors::plotter::PlottingPackets, graphs::scalar_curve::ScalarCurve};

/// This is the gui app of the plotting service.
pub struct PlotterGuiState {
    pub receiver: std::sync::mpsc::Receiver<PlottingPackets>,

    /// holds a collection of plots indexed by plot_name
    pub plots: std::collections::BTreeMap<String, Plot>,
    toggle_plot: std::collections::BTreeMap<String, bool>,
    selected_plot: String,

    show_axis: [bool; 3],
}

impl Into<egui::Color32> for crate::graphs::common::Color {
    fn into(self) -> egui::Color32 {
        egui::Color32::from_rgba_premultiplied(
            (self.r * 255.0) as u8,
            (self.g * 255.0) as u8,
            (self.b * 255.0) as u8,
            (self.a * 255.0) as u8,
        )
    }
}

#[derive(Clone, Debug)]
pub enum GraphType {
    Scalar(ScalarCurve),
}

#[derive(Clone, Debug)]
pub struct CurveStruct {
    pub curve: GraphType,
    pub toggle: bool,
}

/// a single plot is a collection of curves, indexed by curve_name
#[derive(Clone, Debug)]
pub struct Plot {
    pub curves: std::collections::BTreeMap<String, CurveStruct>,
    pub x_range: Option<(f64, f64)>,
    pub y_range: Option<(f64, f64)>,
}

impl PlotterGuiState {
    pub fn new(receiver: std::sync::mpsc::Receiver<PlottingPackets>) -> Self {
        PlotterGuiState {
            receiver,
            plots: std::collections::BTreeMap::new(),
            toggle_plot: std::collections::BTreeMap::new(),
            show_axis: [true; 3],
            selected_plot: String::new(),
        }
    }
}

impl eframe::App for PlotterGuiState {
    /// Called by eframe to update the app
    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        for msg_vec in self.receiver.try_iter() {
            for msg in msg_vec {
                match msg {
                    crate::actors::plotter::PlottingPacket::ScalarCurve(new_value) => {
                        let plot_name = new_value.plot_name.clone();
                        let curve_name = new_value.graph_name.clone();

                        let plot = self.plots.entry(plot_name.clone()).or_insert(Plot {
                            curves: std::collections::BTreeMap::new(),
                            x_range: None,
                            y_range: None,
                        });
                        plot.curves
                            .entry(curve_name.clone())
                            .and_modify(|curve_struct| match &mut curve_struct.curve {
                                GraphType::Scalar(g) => {
                                    g.append(
                                        new_value.scalar_curve.data.clone(),
                                        new_value.scalar_curve.color.clone(),
                                        new_value.scalar_curve.curve_type.clone(),
                                        None,
                                    );
                                }
                            })
                            .or_insert(CurveStruct {
                                curve: GraphType::Scalar(new_value.scalar_curve),
                                toggle: true,
                            });

                        self.toggle_plot.insert(plot_name, true);
                    }
                }
            }
        }
        // match self.receiver.try_recv() {
        //     Ok(msg) => match msg {
        //         PlotterMessage::ScalarCurve(new_value) => {
        //             let plot_name = new_value.plot_name.clone();
        //             let curve_name = new_value.graph_name.clone();

        //             let plot = self.plots.entry(plot_name.clone()).or_insert(Plot {
        //                 curves: std::collections::BTreeMap::new(),
        //                 x_range: None,
        //                 y_range: None,
        //             });
        //             plot.curves
        //                 .entry(curve_name.clone())
        //                 .and_modify(|curve_struct| match &mut curve_struct.curve {
        //                     GraphType::Scalar(g) => {
        //                         g.append(
        //                             new_value.scalar_curve.data.clone(),
        //                             new_value.scalar_curve.color.clone(),
        //                             new_value.scalar_curve.curve_type.clone(),
        //                             None,
        //                         );
        //                     }
        //                 })
        //                 .or_insert(CurveStruct {
        //                     curve: GraphType::Scalar(new_value.scalar_curve),
        //                     toggle: true,
        //                 });

        //             self.toggle_plot.insert(plot_name, true);
        //         }
        //     },
        //     Err(_) => {}
        // }

        egui::SidePanel::left("toggles").show(ctx, |ui| {
            {
                for (plot_name, mut checked) in &mut self.toggle_plot {
                    ui.horizontal(|ui| {
                        ui.checkbox(&mut checked, "");
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
                ui.label(plot_name);
                for (curve_name, curve) in &mut plot.curves {
                    ui.checkbox(&mut curve.toggle, curve_name);
                }
            }

            ui.separator();

            // Toggle axes
            {
                for i in 0..3 {
                    let mut axis = &mut self.show_axis[i];
                    ui.toggle_value(&mut axis, format!("show axis-{}", i));
                }

                if ui.add(egui::Button::new("reset")).clicked() {
                    self.plots = std::collections::BTreeMap::new();
                    self.toggle_plot = std::collections::BTreeMap::new();
                }
            }
        });

        egui::CentralPanel::default().show(ctx, |ui| {
            // 1. Get the available height
            let h = ui.available_height();

            let num_checked = self.toggle_plot.values().filter(|x| **x).count();

            // 2.Calculate the height per plot
            let height_per_plot = (h / num_checked as f32).floor();

            // 3. Plot each plot to the ui

            for (plot_name, plot_data) in &mut self.plots {
                if !self.toggle_plot[plot_name] {
                    continue;
                }
                egui::plot::Plot::new(plot_name)
                    .height(height_per_plot as f32)
                    .legend(
                        egui::plot::Legend::default()
                            .position(egui::widgets::plot::Corner::LeftTop),
                    )
                    .auto_bounds_x()
                    .auto_bounds_y()
                    .show(ui, |plot_ui| {
                        for (curve_name, graph_data) in &mut plot_data.curves {
                            if !graph_data.toggle {
                                continue;
                            }
                            match &graph_data.curve {
                                GraphType::Scalar(g) => {
                                    let mut points = vec![];

                                    for (x, y) in &g.data {
                                        points.push(egui::plot::PlotPoint::new(*x, *y));
                                    }
                                    let plot_points = egui::plot::PlotPoints::Owned(points);

                                    if g.curve_type == "points" {
                                        plot_ui.points(
                                            egui::plot::Points::new(plot_points)
                                                .color(g.color)
                                                .name(curve_name),
                                        );
                                    } else if g.curve_type == "line_strip" {
                                        plot_ui.line(
                                            egui::plot::Line::new(plot_points)
                                                .color(g.color)
                                                .name(curve_name),
                                        );
                                    } else {
                                        println!("Unknown plot type: {}", g.curve_type);
                                    }
                                }
                            }
                        }
                    });
            }
        });

        ctx.request_repaint_after(std::time::Duration::from_secs_f64(0.1));
    }
}
