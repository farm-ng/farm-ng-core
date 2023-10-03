use crate::grpc::farm_ng::core::plotting::proto as grpc_protos;
use crate::plot_gui::*;
use eframe::egui;

/// a curve is a collection of (x,y) pairs, a color, and a plot type
#[derive(Clone, Debug)]
pub struct ScalarCurve {
    pub data: Vec<(f64, f64)>,
    pub color: egui::Color32,
    pub curve_type: String,
}

#[derive(Clone, Debug)]
pub struct RectPlot {
    pub data: Vec<egui::plot::Bar>,
}

impl RectPlot {
    pub fn new(data: Vec<egui::plot::Bar>) -> Self {
        RectPlot { data }
    }

    pub fn append(
        &mut self,
        mut rects: Vec<egui::plot::Bar>,
        clear_x_smaller_than: Option<f64>,
    ) {
        self.data.append(&mut rects);

        if clear_x_smaller_than.is_some() {
            self.drain_filter(clear_x_smaller_than.unwrap());
        }
    }

    pub fn drain_filter(&mut self, clear_x_smaller_than: f64) {
        let mut i = 0;

        while i < self.data.len() {
            if self.data[i].argument < clear_x_smaller_than {
                self.data.remove(i);
            } else {
                i += 1;
            }
        }
    }
}

impl ScalarCurve {
    pub fn new(data: Vec<(f64, f64)>, color: egui::Color32, curve_type: String) -> Self {
        ScalarCurve {
            data,
            color,
            curve_type,
        }
    }

    pub fn append(
        &mut self,
        mut pairs_as_f64: Vec<(f64, f64)>,
        color: egui::Color32,
        curve_type: String,
        clear_x_smaller_than: Option<f64>,
    ) {
        self.data.append(&mut pairs_as_f64);
        self.color = color;
        self.curve_type = curve_type;

        if clear_x_smaller_than.is_some() {
            self.drain_filter(clear_x_smaller_than.unwrap());
        }
    }

    pub fn drain_filter(&mut self, clear_x_smaller_than: f64) {
        let mut i = 0;
        while i < self.data.len() {
            if self.data[i].0 < clear_x_smaller_than {
                self.data.remove(i);
            } else {
                i += 1;
            }
        }
    }
}

#[derive(Clone, Debug)]
pub struct Vec3Curve {
    pub data: Vec<(f64, [f64; 3])>,
    pub color: [egui::Color32; 3],
    pub curve_type: String,
}

impl Vec3Curve {
    pub fn new(data: Vec<(f64, [f64; 3])>, color: [egui::Color32; 3], curve_type: String) -> Self {
        Vec3Curve {
            data,
            color,
            curve_type,
        }
    }

    pub fn append(
        &mut self,
        mut pairs_as_f64: Vec<(f64, [f64; 3])>,
        color: [egui::Color32; 3],
        curve_type: String,
        clear_x_smaller_than: Option<f64>,
    ) {
        self.data.append(&mut pairs_as_f64);
        self.color = color;
        self.curve_type = curve_type;

        if clear_x_smaller_than.is_some() {
            self.drain_filter(clear_x_smaller_than.unwrap());
        }
    }

    pub fn drain_filter(&mut self, clear_x_smaller_than: f64) {
        let mut i = 0;
        while i < self.data.len() {
            if self.data[i].0 < clear_x_smaller_than {
                self.data.remove(i);
            } else {
                i += 1;
            }
        }
    }
}

#[derive(Clone, Debug)]
pub struct Vec3CurveWithConfInterval {
    pub data: Vec<(f64, [f64; 3], [f64; 3])>,
    pub color: [egui::Color32; 3],
    pub conf_color: [egui::Color32; 3],
    pub curve_type: String,
}

impl Vec3CurveWithConfInterval {
    pub fn new(
        data: Vec<(f64, [f64; 3], [f64; 3])>,
        color: [egui::Color32; 3],
        conf_color: [egui::Color32; 3],
        curve_type: String,
    ) -> Self {
        Vec3CurveWithConfInterval {
            data,
            color,
            conf_color,
            curve_type,
        }
    }

    pub fn append(
        &mut self,
        mut pairs_as_f64: Vec<(f64, [f64; 3], [f64; 3])>,
        color: [egui::Color32; 3],
        conf_color: [egui::Color32; 3],
        curve_type: String,
        clear_x_smaller_than: Option<f64>,
    ) {
        self.data.append(&mut pairs_as_f64);
        self.color = color;
        self.conf_color = conf_color;
        self.curve_type = curve_type;

        if clear_x_smaller_than.is_some() {
            self.drain_filter(clear_x_smaller_than.unwrap());
        }
    }

    pub fn drain_filter(&mut self, clear_x_smaller_than: f64) {
        let mut i = 0;
        while i < self.data.len() {
            if self.data[i].0 < clear_x_smaller_than {
                self.data.remove(i);
            } else {
                i += 1;
            }
        }
    }
}

/// Message sent through  tokio::sync::mpsc channel from the service to the app
#[derive(Clone, Debug)]
pub struct UpdateMessages {
    pub proto_messages: grpc_protos::Messages,
}

impl UpdateMessages {
    /// Forward message to the appropriate handler
    pub fn handle(
        &self,
        plots: &mut std::collections::BTreeMap<String, Plot>,
        toggle_graphs: &mut std::collections::BTreeMap<String, bool>,
    ) {
        let proto: &grpc_protos::Messages = &self.proto_messages;

        for m in &proto.messages {
            match m.variant.clone().unwrap() {
                grpc_protos::message::Variant::Curve(g) => {
                    Self::place_curve(&g, plots, toggle_graphs);
                }
                grpc_protos::message::Variant::Vec3Curve(g) => {
                    Self::place_vec3_curve(&g, plots, toggle_graphs);
                }
                grpc_protos::message::Variant::Vec3ConfCurve(g) => {
                    Self::place_vec3_conf_curve(&g, plots, toggle_graphs);
                }
                grpc_protos::message::Variant::Rects(g) => {
                    Self::place_rects(&g, plots, toggle_graphs);
                }
                grpc_protos::message::Variant::XRange(g) => {
                    Self::set_x_range(&g, plots, toggle_graphs);
                }
                grpc_protos::message::Variant::YRange(g) => {
                    Self::set_y_range(&g, plots, toggle_graphs);
                }
            }
        }
    }

    /// Updates existing plot or creates a new one given provided curve
    fn place_curve(
        curve: &grpc_protos::Curve,
        plots: &mut std::collections::BTreeMap<String, Plot>,
        toggle_graphs: &mut std::collections::BTreeMap<String, bool>,
    ) {
        // 1. Parse the curve path into plot_name and curve_name
        let path = &curve.path.clone().unwrap().path_string;
        let tokens = path.split("/").collect::<Vec<&str>>();
        assert_eq!(
            tokens.len(),
            2,
            "path must be of the form `plot_name/curve_name`, got '{}'",
            path
        );
        let plot_name = tokens[0];
        let curve_name = tokens[1];

        if !toggle_graphs.contains_key(plot_name) {
            toggle_graphs.insert(plot_name.to_owned(), true);
        }

        // 2. Parse the curve data

        // num_pairs is the number of (x,y) pairs
        let num_pairs: usize = curve.x_y_pairs.clone().unwrap().num_pairs as usize;
        if num_pairs > 500 {
            println!("Warning: received {} pairs to append, which is a lot. Consider reducing the number of points sent.", num_pairs);
        }
        // pairs_as_bytes is the raw bytes of the (x,y) pairs
        let pairs_as_bytes: &Vec<u8> = &curve.x_y_pairs.clone().unwrap().data;

        use std::mem::size_of;
        assert_eq!(
                num_pairs * 2 * size_of::<f64>(),
                pairs_as_bytes.len(),
                "number of bytes in x_y_pairs.data must be 16 * num_pairs, with 16 = 2 * 8 bytes per f64"
            );

        // Convert bytes to (f64,f64) using f64::from_ne_bytes
        let pairs_as_f64: Vec<(f64, f64)> = pairs_as_bytes
            .chunks_exact(size_of::<f64>() * 2)
            .map(|b| {
                (
                    f64::from_ne_bytes(b[0..size_of::<f64>()].try_into().unwrap()),
                    f64::from_ne_bytes(b[size_of::<f64>()..].try_into().unwrap()),
                )
            })
            .collect();

        // 3. Parse the curve color
        let color = egui::Color32::from_rgb(
            (curve.color.clone().unwrap().r * 255.0) as u8,
            (curve.color.clone().unwrap().g * 255.0) as u8,
            (curve.color.clone().unwrap().b * 255.0) as u8,
        );

        // 4. Parse the clear flag
        let mut clear_x_smaller_than: Option<f64> = None;

        if curve.reset.clone().unwrap().shall_clear {
            clear_x_smaller_than = Some(curve.reset.clone().unwrap().clear_x_smaller_than);
        }

        // 5. Update the plot or create a new one
        if plots.contains_key(plot_name) {
            // if the plot already exists, update it
            let plot = plots.get_mut(plot_name).unwrap();
            if plot.curves.contains_key(curve_name) {
                // if the curve already exists, update it
                match &mut plot.curves.get_mut(curve_name).unwrap().curve {
                    CurveType::Scalar(curve) => curve.append(
                        pairs_as_f64,
                        color,
                        curve.curve_type.clone(),
                        clear_x_smaller_than,
                    ),
                    _ => panic!("CurveStruct type mismatch"),
                }
            } else {
                // if the curve not exist, create it
                plot.curves.insert(
                    curve_name.to_owned(),
                    CurveStruct {
                        curve: CurveType::Scalar(ScalarCurve::new(
                            pairs_as_f64,
                            color,
                            curve.line_type.clone().unwrap().variant_name,
                        )),
                        toggle: true,
                    },
                );
            }
        } else {
            // if the plot does not exist, create it
            let mut plot = Plot {
                curves: std::collections::BTreeMap::new(),
                x_range: None,
                y_range: None,
            };
            // populate plot with given curve
            plot.curves.insert(
                curve_name.to_owned(),
                CurveStruct {
                    curve: CurveType::Scalar(ScalarCurve::new(
                        pairs_as_f64,
                        color,
                        curve.line_type.clone().unwrap().variant_name,
                    )),
                    toggle: true,
                },
            );
            plots.insert(plot_name.to_owned(), plot);
        }
    }

    fn place_vec3_curve(
        curve: &grpc_protos::Vec3Curve,
        plots: &mut std::collections::BTreeMap<String, Plot>,
        toggle_graphs: &mut std::collections::BTreeMap<String, bool>,
    ) {
        // 1. Parse the curve path into plot_name and curve_name
        let path = &curve.path.clone().unwrap().path_string;
        let tokens = path.split("/").collect::<Vec<&str>>();
        assert_eq!(
            tokens.len(),
            2,
            "path must be of the form `plot_name/curve_name`, got {}",
            path
        );
        let plot_name = tokens[0];
        let curve_name = tokens[1];

        if !toggle_graphs.contains_key(plot_name) {
            toggle_graphs.insert(plot_name.to_owned(), true);
        }

        // 2. Parse the curve data

        // num_tuples is the number of tuples
        let num_tuples: usize = curve.x_vec_pairs.clone().unwrap().num_tuples as usize;
        if num_tuples > 500 {
            println!("Warning: received {} pairs to append, which is a lot. Consider reducing the number of points sent.", num_tuples);
        }
        // pairs_as_bytes is the raw bytes of the (x,y) pairs
        let pairs_as_bytes: &Vec<u8> = &curve.x_vec_pairs.clone().unwrap().data;

        use std::mem::size_of;
        assert_eq!(
            num_tuples * 4 * size_of::<f64>(),
           pairs_as_bytes.len(),
           "number of bytes in x_vec_pairs.data must be 32 * num_tuples, with 32 = 4 * 8 bytes per f64"
       );

        // Convert bytes to (f64,f64) using f64::from_ne_bytes
        let pairs_as_f64: Vec<(f64, (f64, f64, f64))> = pairs_as_bytes
            .chunks_exact(size_of::<f64>() * 4)
            .map(|b| {
                (
                    f64::from_ne_bytes(b[0..size_of::<f64>()].try_into().unwrap()),
                    (
                        f64::from_ne_bytes(
                            b[size_of::<f64>()..2 * size_of::<f64>()]
                                .try_into()
                                .unwrap(),
                        ),
                        f64::from_ne_bytes(
                            b[2 * size_of::<f64>()..3 * size_of::<f64>()]
                                .try_into()
                                .unwrap(),
                        ),
                        f64::from_ne_bytes(b[3 * size_of::<f64>()..].try_into().unwrap()),
                    ),
                )
            })
            .collect();
        let mut v: Vec<(f64, [f64; 3])> = Vec::new();

        for (x, y) in pairs_as_f64 {
            v.push((x, [y.0, y.1, y.2]));
        }

        // 3. Parse the curve color
        let color_0 = egui::Color32::from_rgb(
            (curve.color.clone()[0].r * 255.0) as u8,
            (curve.color.clone()[0].g * 255.0) as u8,
            (curve.color.clone()[0].b * 255.0) as u8,
        );
        let color_1 = egui::Color32::from_rgb(
            (curve.color.clone()[1].r * 255.0) as u8,
            (curve.color.clone()[1].g * 255.0) as u8,
            (curve.color.clone()[1].b * 255.0) as u8,
        );
        let color_2 = egui::Color32::from_rgb(
            (curve.color.clone()[2].r * 255.0) as u8,
            (curve.color.clone()[2].g * 255.0) as u8,
            (curve.color.clone()[2].b * 255.0) as u8,
        );

        // 4. Parse the clear flag
        let mut clear_x_smaller_than: Option<f64> = None;
        if curve.reset.clone().unwrap().shall_clear {
            clear_x_smaller_than = Some(curve.reset.clone().unwrap().clear_x_smaller_than);
        }

        // 5. Update the plot or create a new one
        if plots.contains_key(plot_name) {
            // if the plot already exists, update it
            let plot = plots.get_mut(plot_name).unwrap();
            //let mut Vec3Curve curve = plot.curves.get_mut(curve_name).unwrap();

            if plot.curves.contains_key(curve_name) {
                // if the curve already exists, update it
                match &mut plot.curves.get_mut(curve_name).unwrap().curve {
                    CurveType::Vec3(curve) => curve.append(
                        v,
                        [color_0, color_1, color_2],
                        curve.curve_type.clone(),
                        clear_x_smaller_than,
                    ),
                    _ => panic!("CurveStruct type mismatch"),
                }
            } else {
                // if the curve does not exist, create it
                plot.curves.insert(
                    curve_name.to_owned(),
                    CurveStruct {
                        curve: CurveType::Vec3(Vec3Curve::new(
                            v,
                            [color_0, color_1, color_2],
                            curve.line_type.clone().unwrap().variant_name,
                        )),
                        toggle: true,
                    },
                );
            }
        } else {
            // if the plot does not exist, create it
            let mut plot = Plot {
                curves: std::collections::BTreeMap::new(),
                x_range: None,
                y_range: None,
            };
            // populate plot with given curve
            plot.curves.insert(
                curve_name.to_owned(),
                CurveStruct {
                    curve: CurveType::Vec3(Vec3Curve::new(
                        v,
                        [color_0, color_1, color_2],
                        curve.line_type.clone().unwrap().variant_name,
                    )),
                    toggle: true,
                },
            );
            plots.insert(plot_name.to_owned(), plot);
        }
    }

    fn place_vec3_conf_curve(
        curve: &grpc_protos::Vec3CurveWithConfInterval,
        plots: &mut std::collections::BTreeMap<String, Plot>,
        toggle_graphs: &mut std::collections::BTreeMap<String, bool>,
    ) {
        // 1. Parse the curve path into plot_name and curve_name
        let path = &curve.path.clone().unwrap().path_string;
        let tokens = path.split("/").collect::<Vec<&str>>();
        assert_eq!(
            tokens.len(),
            2,
            "path must be of the form `plot_name/curve_name`, got {}",
            path
        );
        let plot_name = tokens[0];
        let curve_name = tokens[1];

        if !toggle_graphs.contains_key(plot_name) {
            toggle_graphs.insert(plot_name.to_owned(), true);
        }

        // 2. Parse the curve data

        // num_pairs is the number of (x,y) pairs
        let num_pairs: usize = curve.x_vec_conf_tuples.clone().unwrap().num_tuples as usize;
        if num_pairs > 500 {
            println!("Warning: received {} pairs to append, which is a lot. Consider reducing the number of points sent.", num_pairs);
        }
        // pairs_as_bytes is the raw bytes of the (x,y) pairs
        let pairs_as_bytes: &Vec<u8> = &curve.x_vec_conf_tuples.clone().unwrap().data;

        use std::mem::size_of;
        assert_eq!(
            num_pairs * 7 * size_of::<f64>(),
            pairs_as_bytes.len(),
            "number of bytes in x_vec_conf_tuples.data must ..."
        );

        // Convert bytes to (f64,f64) using f64::from_ne_bytes
        let pairs_as_f64: Vec<(f64, (f64, f64, f64), (f64, f64, f64))> = pairs_as_bytes
            .chunks_exact(size_of::<f64>() * 7)
            .map(|b| {
                (
                    f64::from_ne_bytes(b[0..size_of::<f64>()].try_into().unwrap()),
                    (
                        f64::from_ne_bytes(
                            b[size_of::<f64>()..2 * size_of::<f64>()]
                                .try_into()
                                .unwrap(),
                        ),
                        f64::from_ne_bytes(
                            b[2 * size_of::<f64>()..3 * size_of::<f64>()]
                                .try_into()
                                .unwrap(),
                        ),
                        f64::from_ne_bytes(
                            b[3 * size_of::<f64>()..4 * size_of::<f64>()]
                                .try_into()
                                .unwrap(),
                        ),
                    ),
                    (
                        f64::from_ne_bytes(
                            b[4 * size_of::<f64>()..5 * size_of::<f64>()]
                                .try_into()
                                .unwrap(),
                        ),
                        f64::from_ne_bytes(
                            b[5 * size_of::<f64>()..6 * size_of::<f64>()]
                                .try_into()
                                .unwrap(),
                        ),
                        f64::from_ne_bytes(
                            b[6 * size_of::<f64>()..7 * size_of::<f64>()]
                                .try_into()
                                .unwrap(),
                        ),
                    ),
                )
            })
            .collect();
        let mut v: Vec<(f64, [f64; 3], [f64; 3])> = Vec::new();

        for (x, y, z) in pairs_as_f64 {
            v.push((x, [y.0, y.1, y.2], [z.0, z.1, z.2]));
        }

        // 3. Parse the curve color
        let color_0 = egui::Color32::from_rgb(
            (curve.color.clone()[0].r * 255.0) as u8,
            (curve.color.clone()[0].g * 255.0) as u8,
            (curve.color.clone()[0].b * 255.0) as u8,
        );
        let color_1 = egui::Color32::from_rgb(
            (curve.color.clone()[1].r * 255.0) as u8,
            (curve.color.clone()[1].g * 255.0) as u8,
            (curve.color.clone()[1].b * 255.0) as u8,
        );
        let color_2 = egui::Color32::from_rgb(
            (curve.color.clone()[2].r * 255.0) as u8,
            (curve.color.clone()[2].g * 255.0) as u8,
            (curve.color.clone()[2].b * 255.0) as u8,
        );

        let ccolor_0 = egui::Color32::from_rgb(
            (curve.conf_color.clone()[0].r * 255.0) as u8,
            (curve.conf_color.clone()[0].g * 255.0) as u8,
            (curve.conf_color.clone()[0].b * 255.0) as u8,
        );
        let ccolor_1 = egui::Color32::from_rgb(
            (curve.conf_color.clone()[1].r * 255.0) as u8,
            (curve.conf_color.clone()[1].g * 255.0) as u8,
            (curve.conf_color.clone()[1].b * 255.0) as u8,
        );
        let ccolor_2 = egui::Color32::from_rgb(
            (curve.conf_color.clone()[2].r * 255.0) as u8,
            (curve.conf_color.clone()[2].g * 255.0) as u8,
            (curve.conf_color.clone()[2].b * 255.0) as u8,
        );

        // 4. Parse the clear flag
        let mut clear_x_smaller_than: Option<f64> = None;
        if curve.reset.clone().unwrap().shall_clear {
            clear_x_smaller_than = Some(curve.reset.clone().unwrap().clear_x_smaller_than);
        }

        // 5. Update the plot or create a new one
        if plots.contains_key(plot_name) {
            // if the plot already exists, update it
            let plot = plots.get_mut(plot_name).unwrap();
            //let mut Vec3Curve curve = plot.curves.get_mut(curve_name).unwrap();

            if plot.curves.contains_key(curve_name) {
                // if the curve already exists, update it
                match &mut plot.curves.get_mut(curve_name).unwrap().curve {
                    CurveType::Vec3Conf(curve) => curve.append(
                        v,
                        [color_0, color_1, color_2],
                        [ccolor_0, ccolor_1, ccolor_2],
                        curve.curve_type.clone(),
                        clear_x_smaller_than,
                    ),
                    _ => panic!("CurveStruct type mismatch"),
                }
            } else {
                // if the curve does not exist, create it
                plot.curves.insert(
                    curve_name.to_owned(),
                    CurveStruct {
                        curve: CurveType::Vec3Conf(Vec3CurveWithConfInterval::new(
                            v,
                            [color_0, color_1, color_2],
                            [ccolor_0, ccolor_1, ccolor_2],
                            curve.line_type.clone().unwrap().variant_name,
                        )),
                        toggle: true,
                    },
                );
            }
        } else {
            // if the plot does not exist, create it
            let mut plot = Plot {
                curves: std::collections::BTreeMap::new(),
                x_range: None,
                y_range: None,
            };
            // populate plot with given curve
            plot.curves.insert(
                curve_name.to_owned(),
                CurveStruct {
                    curve: CurveType::Vec3Conf(Vec3CurveWithConfInterval::new(
                        v,
                        [color_0, color_1, color_2],
                        [ccolor_0, ccolor_1, ccolor_2],
                        curve.line_type.clone().unwrap().variant_name,
                    )),
                    toggle: true,
                },
            );
            plots.insert(plot_name.to_owned(), plot);
        }
    }

    fn set_x_range(
        x_range: &grpc_protos::XRange,
        plots: &mut std::collections::BTreeMap<String, Plot>,
        toggle_graphs: &mut std::collections::BTreeMap<String, bool>,
    ) {
        // 1. Parse the curve path into plot_name and curve_name
        let path = &x_range.clone().path.unwrap().path_string;
        let tokens = path.split("/").collect::<Vec<&str>>();
        assert!(tokens.len() >= 1, "got {}", path);

        // 2. Parse the x_range
        let range = (
            x_range.clone().range.unwrap().min,
            x_range.clone().range.unwrap().max,
        );
        let plot_name = tokens[0];

        if !toggle_graphs.contains_key(plot_name) {
            toggle_graphs.insert(plot_name.to_owned(), true);
        }

        // 3. Update the plot or create a new one
        if plots.contains_key(plot_name) {
            let plot = plots.get_mut(plot_name).unwrap();
            plot.x_range = Some(range);
        } else {
            let plot = Plot {
                curves: std::collections::BTreeMap::new(),
                x_range: Some(range),
                y_range: None,
            };
            plots.insert(plot_name.to_owned(), plot);
        }
    }

    fn set_y_range(
        y_range: &grpc_protos::YRange,
        plots: &mut std::collections::BTreeMap<String, Plot>,
        toggle_graphs: &mut std::collections::BTreeMap<String, bool>,
    ) {
        // 1. Parse the curve path into plot_name and curve_name
        let path = &y_range.clone().path.unwrap().path_string;
        let tokens = path.split("/").collect::<Vec<&str>>();
        assert!(tokens.len() >= 1, "got {}", path);

        // 2. Parse the y_range
        let range = (
            y_range.range.clone().unwrap().min,
            y_range.range.clone().unwrap().max,
        );
        let plot_name = tokens[0];

        if !toggle_graphs.contains_key(plot_name) {
            toggle_graphs.insert(plot_name.to_owned(), true);
        }

        // 3. Update the plot or create a new one
        if plots.contains_key(plot_name) {
            let plot = plots.get_mut(plot_name).unwrap();
            plot.y_range = Some(range);
        } else {
            let plot = Plot {
                curves: std::collections::BTreeMap::new(),
                x_range: None,
                y_range: Some(range),
            };
            plots.insert(plot_name.to_owned(), plot);
        }
    }

    fn place_rects(
        rects: &grpc_protos::RectPlot,
        plots: &mut std::collections::BTreeMap<String, Plot>,
        toggle_graphs: &mut std::collections::BTreeMap<String, bool>,
    ) {
        // 1. Parse the curve path into plot_name and curve_name
        let path = &rects.clone().path.unwrap().path_string;
        let tokens = path.split("/").collect::<Vec<&str>>();
        assert_eq!(
            tokens.len(),
            2,
            "path must be of the form `plot_name/curve_name`, got '{}'",
            path
        );
        let plot_name = tokens[0];
        let curve_name = tokens[1];

        if !toggle_graphs.contains_key(plot_name) {
            toggle_graphs.insert(plot_name.to_owned(), true);
        }

        // 2. Parse the curve data

        // num_pairs is the number of rects
        let num_rects: usize = rects.colored_rects.clone().unwrap().value.len() as usize;
        if num_rects > 500 {
            println!("Warning: received {} pairs to append, which is a lot. Consider reducing the number of rects sent.", num_rects);
        }

        let raw_rects: &Vec<grpc_protos::ColoredRect> = &rects.colored_rects.clone().unwrap().value;

        let mut bars = Vec::new();

        for r in raw_rects {
            let min_x = r.region.clone().unwrap().min.unwrap().x;
            let max_x = r.region.clone().unwrap().max.unwrap().x;
            let x = (min_x + max_x) / 2.0;
            let min_y = r.region.clone().unwrap().min.unwrap().y;
            let max_y = r.region.clone().unwrap().max.unwrap().y;

            let bar = egui::plot::Bar::new(x, max_y - min_y)
                .base_offset(min_y)
                .width(max_x - min_x)
                .fill(egui::Color32::from_rgb(
                    (r.color.clone().unwrap().r * 255.0) as u8,
                    (r.color.clone().unwrap().g * 255.0) as u8,
                    (r.color.clone().unwrap().b * 255.0) as u8,
                ));
            bars.push(bar);
        }

        // 4. Parse the clear flag
        let mut clear_x_smaller_than: Option<f64> = None;

        if rects.reset.clone().unwrap().shall_clear {
            clear_x_smaller_than = Some(rects.reset.clone().unwrap().clear_x_smaller_than);
        }

        // 5. Update the plot or create a new one
        if plots.contains_key(plot_name) {
            // if the plot already exists, update it
            let plot = plots.get_mut(plot_name).unwrap();
            if plot.curves.contains_key(curve_name) {
                // if the curve already exists, update it
                match &mut plot.curves.get_mut(curve_name).unwrap().curve {
                    CurveType::Rect(r) => r.append(bars, clear_x_smaller_than),
                    _ => panic!("CurveStruct type mismatch"),
                }
            } else {
                // if the curve not exist, create it
                plot.curves.insert(
                    curve_name.to_owned(),
                    CurveStruct {
                        curve: CurveType::Rect(RectPlot::new(bars)),
                        toggle: true,
                    },
                );
            }
        } else {
            // if the plot does not exist, create it
            let mut plot = Plot {
                curves: std::collections::BTreeMap::new(),
                x_range: None,
                y_range: None,
            };
            // populate plot with given curve
            plot.curves.insert(
                curve_name.to_owned(),
                CurveStruct {
                    curve: CurveType::Rect(RectPlot::new(bars)),
                    toggle: true,
                },
            );
            plots.insert(plot_name.to_owned(), plot);
        }
    }
}
