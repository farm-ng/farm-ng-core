use crate::message::RectPlot;
use crate::message::ScalarCurve;
use crate::message::Vec3CurveWithConfInterval;
use crate::message::Vec3Curve;

use crate::plotting_service::UpdateReceiver;
use eframe::egui;

/// This is the gui app of the plotting service.
pub struct PlottingApp {
    update_receiver: UpdateReceiver,
    /// holds a collection of plots indexed by plot_name
    pub plots: std::collections::BTreeMap<String, Plot>,
    toggle_plot: std::collections::BTreeMap<String, bool>,
    selected_plot: String,

    show_axis: [bool; 3],
}

#[derive(Clone, Debug)]
pub enum CurveType {
    Scalar(ScalarCurve),
    Vec3(Vec3Curve),
    Vec3Conf(Vec3CurveWithConfInterval),
    Rect(RectPlot),
}

#[derive(Clone, Debug)]
pub struct CurveStruct {
    pub curve: CurveType,
    pub toggle: bool,
}

/// a single plot is a collection of curves, indexed by curve_name
#[derive(Clone, Debug)]
pub struct Plot {
    pub curves: std::collections::BTreeMap<String, CurveStruct>,
    pub x_range: Option<(f64, f64)>,
    pub y_range: Option<(f64, f64)>,
}

impl PlottingApp {
    /// Create a new app with the given receiver
    pub fn new(update_receiver: UpdateReceiver) -> Self {
        PlottingApp {
            update_receiver,
            plots: std::collections::BTreeMap::new(),
            toggle_plot: std::collections::BTreeMap::new(),
            show_axis: [true; 3],
            selected_plot: String::new(),
        }
    }
}

impl eframe::App for PlottingApp {
    /// Called by eframe to update the app
    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        match self.update_receiver.try_recv() {
            Ok(msg) => {
                msg.handle(&mut self.plots, &mut self.toggle_plot);
            }
            Err(_) => {}
        }

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
                                CurveType::Scalar(g) => {
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
                                CurveType::Vec3(g) => {
                                    for i in 0..3 {
                                        if !self.show_axis[i] {
                                            continue;
                                        }
                                        let mut points_i = vec![];

                                        for (x, v) in &g.data {
                                            points_i.push(egui::plot::PlotPoint::new(*x, v[i]));
                                        }

                                        let plot_points_i = egui::plot::PlotPoints::Owned(points_i);

                                        if g.curve_type == "points" {
                                            plot_ui.points(
                                                egui::plot::Points::new(plot_points_i)
                                                    .color(g.color[i])
                                                    .name(format!("{}-{}", curve_name, i)),
                                            );
                                        } else if g.curve_type == "line_strip" {
                                            plot_ui.line(
                                                egui::plot::Line::new(plot_points_i)
                                                    .color(g.color[i])
                                                    .name(format!("{}-{}", curve_name, i)),
                                            );
                                        } else {
                                            println!("Unknown plot type: {}", g.curve_type);
                                        }
                                    }
                                }
                                CurveType::Vec3Conf(g) => {
                                    for i in 0..3 {
                                        if !self.show_axis[i] {
                                            continue;
                                        }
                                        let mut points_i = vec![];
                                        let mut points_p_i = vec![];
                                        let mut points_m_i = vec![];

                                        for (x, v, c) in &g.data {
                                            points_i.push(egui::plot::PlotPoint::new(*x, v[i]));
                                            points_p_i
                                                .push(egui::plot::PlotPoint::new(*x, v[i] + c[i]));
                                            points_m_i
                                                .push(egui::plot::PlotPoint::new(*x, v[i] - c[i]));
                                        }

                                        let plot_points_i = egui::plot::PlotPoints::Owned(points_i);
                                        let plot_points_p_i =
                                            egui::plot::PlotPoints::Owned(points_p_i);
                                        let plot_points_m_i =
                                            egui::plot::PlotPoints::Owned(points_m_i);

                                        if g.curve_type == "points" {
                                            plot_ui.points(
                                                egui::plot::Points::new(plot_points_i)
                                                    .color(g.color[i])
                                                    .name(format!("{}-{}", curve_name, i)),
                                            );
                                            plot_ui.points(
                                                egui::plot::Points::new(plot_points_p_i)
                                                    .color(g.conf_color[i])
                                            );
                                            plot_ui.points(
                                                egui::plot::Points::new(plot_points_m_i)
                                                    .color(g.conf_color[i])
                                                    .name(format!("{}-{}-", curve_name, i)),
                                            );
                                        } else if g.curve_type == "line_strip" {
                                            plot_ui.line(
                                                egui::plot::Line::new(plot_points_i)
                                                    .color(g.color[i])
                                                    .name(format!("{}-{}", curve_name, i)),
                                            );
                                            plot_ui.line(
                                                egui::plot::Line::new(plot_points_p_i)
                                                    .color(g.conf_color[i]),
                                            );
                                            plot_ui.line(
                                                egui::plot::Line::new(plot_points_m_i)
                                                    .color(g.conf_color[i])
                                            );
                                        } else {
                                            println!("Unknown plot type: {}", g.curve_type);
                                        }
                                    }
                                }
                                CurveType::Rect(r) => {
                                    plot_ui.bar_chart(egui::plot::BarChart::new(r.data.clone()));
                                }
                            }
                        }

                        //  if plot_data.x_range.is_some() || plot_data.y_range.is_some() {
                        //     if plot_data.x_range.is_none() {
                        //         plot_data.x_range = Some((
                        //             plot_ui.plot_bounds().min()[0],
                        //             plot_ui.plot_bounds().max()[0],
                        //         ));
                        //     }
                        //     if plot_data.y_range.is_none() {
                        //         plot_data.y_range = Some((
                        //             plot_ui.plot_bounds().min()[1],
                        //             plot_ui.plot_bounds().max()[1],
                        //         ));
                        //     }
                        //     // now both x_range and y_range are Some
                        //     let x_range = plot_data.x_range.unwrap();
                        //     let y_range = plot_data.y_range.unwrap();

                        //     plot_ui.set_plot_bounds(egui::plot::PlotBounds::from_min_max(
                        //         [x_range.0, y_range.0],
                        //         [x_range.1, y_range.1],
                        //     ));
                        // }
                    });
            }
        });

        ctx.request_repaint_after(std::time::Duration::from_secs_f64(0.1));
    }
}
