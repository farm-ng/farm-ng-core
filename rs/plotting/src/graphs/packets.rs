use super::{
    common::{Bounds, ClearCondition},
    scalar_curve::{NamedScalarCurve, ScalarCurve, ScalarCurveStyle},
    vec3_conf_curve::{NamedVec3ConfCurve, Vec3ConfCurve, Vec3ConfCurveStyle},
    vec3_curve::{NamedVec3Curve, Vec3Curve, Vec3CurveStyle},
};

#[derive(Clone, Debug)]
pub enum PlottingPacket {
    /// a float value
    ScalarCurve(NamedScalarCurve),
    Vec3Curve(NamedVec3Curve),
    Vec3ConfCurve(NamedVec3ConfCurve),
}
pub type PlottingPackets = Vec<PlottingPacket>;

impl PlottingPacket {
    pub fn append_to_curve<S: Into<String>>(
        (plot, graph): (S, S),
        (x, y): (f64, f64),
        style: ScalarCurveStyle ,
        bounds: Bounds,
    ) -> PlottingPacket {
        let curve = NamedScalarCurve {
            plot_name: plot.into(),
            graph_name: graph.into(),
            scalar_curve: ScalarCurve {
                data: vec![(x, y)].into(),
                style,
                clear_cond: ClearCondition::append(),
                bounds,
            },
        };

        PlottingPacket::ScalarCurve(curve)
    }

    pub fn append_to_vec3_curve<S: Into<String>>(
        (plot, graph): (S, S),
        (x, y): (f64, (f64, f64, f64)),
        style: Vec3CurveStyle,
        bounds: Bounds,
    ) -> PlottingPacket {
        let curve = NamedVec3Curve {
            plot_name: plot.into(),
            graph_name: graph.into(),
            scalar_curve: Vec3Curve {
                data: vec![(x, y)].into(),
                style,
                clear_cond: ClearCondition::append(),
                bounds,
            },
        };

        PlottingPacket::Vec3Curve(curve)
    }

    pub fn append_to_vec3_conf_curve<S: Into<String>>(
        (plot, graph): (S, S),
        (x, y): (f64, ((f64, f64, f64), (f64, f64, f64))),
        style: Vec3ConfCurveStyle,
        bounds: Bounds,
    ) -> PlottingPacket {
        let curve = NamedVec3ConfCurve {
            plot_name: plot.into(),
            graph_name: graph.into(),
            scalar_curve: Vec3ConfCurve {
                data: vec![(x, y)].into(),
                style,
                clear_cond: ClearCondition::append(),

                bounds,
            },
        };

        PlottingPacket::Vec3ConfCurve(curve)
    }
}
