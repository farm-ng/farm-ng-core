use super::{
    common::{Bounds, Color, LineType, ResetPredicate},
    scalar_curve::{NamedScalarCurve, ScalarCurve},
    vec3_curve::{NamedVec3Curve, Vec3Curve},
};

#[derive(Clone, Debug)]
pub enum PlottingPacket {
    /// a float value
    ScalarCurve(NamedScalarCurve),
    Vec3Curve(NamedVec3Curve),
}
pub type PlottingPackets = Vec<PlottingPacket>;

impl PlottingPacket {
    pub fn append_to_curve<S: Into<String>>(
        (plot, graph): (S, S),
        color: Color,
        (x, y): (f64, f64),
        history_length: f64,
        bounds: Bounds,
    ) -> PlottingPacket {
        let curve = NamedScalarCurve {
            plot_name: plot.into(),
            graph_name: graph.into(),
            scalar_curve: ScalarCurve {
                data: vec![(x, y)],
                color,
                curve_type: LineType::default(),
                clear_x_smaller_than: ResetPredicate {
                    clear_x_smaller_than: Some(x - history_length),
                },
                bounds,
            },
        };

        PlottingPacket::ScalarCurve(curve)
    }

    pub fn append_to_vec3_curve<S: Into<String>>(
        (plot, graph): (S, S),
        (x, y): (f64, (f64, f64, f64)),
        history_length: f64,
        bounds: Bounds,
    ) -> PlottingPacket {
        let curve = NamedVec3Curve {
            plot_name: plot.into(),
            graph_name: graph.into(),
            scalar_curve: Vec3Curve {
                data: vec![(x, y)],
                color: [Color::red(), Color::green(), Color::blue()],
                curve_type: LineType::default(),
                clear_x_smaller_than: ResetPredicate {
                    clear_x_smaller_than: Some(x - history_length),
                },
                bounds,
            },
        };

        PlottingPacket::Vec3Curve(curve)
    }
}
