use std::mem::size_of;

use crate::graphs::common::{
    Bounds, ClearCondition, LineType, XCoordinateBounds, YCoordinateBounds,
};
use crate::graphs::packets::{PlottingPacket, PlottingPackets};
use crate::graphs::scalar_curve::{NamedScalarCurve, ScalarCurve, ScalarCurveStyle};
use crate::graphs::vec3_conf_curve::{NamedVec3ConfCurve, Vec3ConfCurve, Vec3ConfCurveStyle};
use crate::graphs::vec3_curve::{NamedVec3Curve, Vec3Curve, Vec3CurveStyle};

use crate::grpc::farm_ng::core::plotting::proto;
use crate::grpc::farm_ng::core::proto::OptionalG0Double;
use crate::grpc::proto::proto::Messages;
use std::collections::VecDeque;

pub fn color_from_proto(
    proto: crate::grpc::farm_ng::core::proto::Color,
) -> crate::graphs::common::Color {
    crate::graphs::common::Color {
        r: proto.r,
        g: proto.g,
        b: proto.b,
        a: proto.a,
    }
}

pub fn colors_from_proto(
    proto: Vec<crate::grpc::farm_ng::core::proto::Color>,
) -> [crate::graphs::common::Color; 3] {
    let mut a = [crate::graphs::common::Color::new(); 3];
    for i in 0..3 {
        a[i] = color_from_proto(proto[i].clone());
    }
    a
}

pub fn line_type_from_proto(proto: proto::LineType) -> LineType {
    if proto.variant_name == "line_strip" {
        return LineType::LineStrip;
    }
    LineType::Points
}

pub fn clear_cond_from_proto(proto: proto::ClearCondition) -> ClearCondition {
    ClearCondition {
        max_x_range: proto.max_x_range,
    }
}

pub fn maybe_double_from_proto(proto: OptionalG0Double) -> Option<f64> {
    if !proto.has_value {
        return None;
    }
    Some(proto.value)
}

pub fn bound_from_proto(proto: crate::grpc::farm_ng::core::plotting::proto::Bounds) -> Bounds {
    let proto_x_bounds = proto.x_bounds.unwrap();
    let maybe_max_x = maybe_double_from_proto(proto_x_bounds.max_x.unwrap());
    let x_bounds = XCoordinateBounds {
        data_driven: maybe_max_x.is_none(),
        max_x: maybe_max_x.unwrap_or_default(),
        len: proto_x_bounds.len,
    };
    let proto_y_bounds = proto.y_bounds.unwrap();
    let maybe_height = maybe_double_from_proto(proto_y_bounds.height.unwrap());
    let y_bounds = YCoordinateBounds {
        data_driven: maybe_height.is_none(),
        height: maybe_height.unwrap_or(1.0),
        offset: proto_y_bounds.offset,
    };
    Bounds { x_bounds, y_bounds }
}

pub fn from_proto(m: Messages) -> PlottingPackets {
    let mut p = vec![];
    for mes in m.messages {
        match mes.variant {
            Some(v) => match v {
                crate::grpc::farm_ng::core::plotting::proto::message::Variant::Curve(
                    proto_curve,
                ) => {
                    let path: String = proto_curve.path.unwrap().path_string;
                    let tokens = path.split('/').collect::<Vec<&str>>();
                    assert_eq!(
                        tokens.len(),
                        2,
                        "path must be of the form `plot_name/curve_name`, got '{}'",
                        path
                    );
                    let plot_name = tokens[0];
                    let curve_name = tokens[1];

                    let pairs_as_f64: VecDeque<(f64, f64)> = proto_curve
                        .x_y_pairs
                        .unwrap()
                        .data
                        .chunks_exact(size_of::<f64>() * 2)
                        .map(|b| {
                            (
                                f64::from_ne_bytes(b[0..size_of::<f64>()].try_into().unwrap()),
                                f64::from_ne_bytes(b[size_of::<f64>()..].try_into().unwrap()),
                            )
                        })
                        .collect();

                    p.push(PlottingPacket::ScalarCurve(NamedScalarCurve {
                        plot_name: plot_name.to_owned(),
                        graph_name: curve_name.to_owned(),
                        scalar_curve: ScalarCurve {
                            data: pairs_as_f64,
                            style: ScalarCurveStyle {
                                color: color_from_proto(proto_curve.color.unwrap()),
                                line_type: line_type_from_proto(proto_curve.line_type.unwrap()),
                            },
                            clear_cond: clear_cond_from_proto(proto_curve.clear_cond.unwrap()),
                            bounds: bound_from_proto(proto_curve.bounds.unwrap()),
                        },
                    }));
                }
                crate::grpc::farm_ng::core::plotting::proto::message::Variant::Vec3Curve(
                    proto_curve3,
                ) => {
                    let path: String = proto_curve3.path.unwrap().path_string;
                    let tokens = path.split('/').collect::<Vec<&str>>();
                    assert_eq!(
                        tokens.len(),
                        2,
                        "path must be of the form `plot_name/curve_name`, got '{}'",
                        path
                    );
                    let plot_name = tokens[0];
                    let curve_name = tokens[1];

                    let pairs_as_f64: VecDeque<(f64, (f64, f64, f64))> = proto_curve3
                        .x_vec_pairs
                        .unwrap()
                        .data
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
                                    f64::from_ne_bytes(
                                        b[3 * size_of::<f64>()..].try_into().unwrap(),
                                    ),
                                ),
                            )
                        })
                        .collect();
                    let cs = colors_from_proto(proto_curve3.color);
                    p.push(PlottingPacket::Vec3Curve(NamedVec3Curve {
                        plot_name: plot_name.to_owned(),
                        graph_name: curve_name.to_owned(),
                        scalar_curve: Vec3Curve {
                            data: pairs_as_f64,
                            style: Vec3CurveStyle {
                                color: cs,
                                line_type: line_type_from_proto(proto_curve3.line_type.unwrap()),
                            },
                            clear_cond: clear_cond_from_proto(proto_curve3.clear_cond.unwrap()),
                            bounds: bound_from_proto(proto_curve3.bounds.unwrap()),
                        },
                    }));
                }
                crate::grpc::farm_ng::core::plotting::proto::message::Variant::Vec3ConfCurve(
                    proto_curve3,
                ) => {
                    let path: String = proto_curve3.path.unwrap().path_string;
                    let tokens = path.split('/').collect::<Vec<&str>>();
                    assert_eq!(
                        tokens.len(),
                        2,
                        "path must be of the form `plot_name/curve_name`, got '{}'",
                        path
                    );
                    let plot_name = tokens[0];
                    let curve_name = tokens[1];

                    // Convert bytes to (f64,f64) using f64::from_ne_bytes
                    let pairs_as_f64: VecDeque<(f64, ((f64, f64, f64), (f64, f64, f64)))> =
                        proto_curve3
                            .x_vec_conf_tuples
                            .unwrap()
                            .data
                            .chunks_exact(size_of::<f64>() * 7)
                            .map(|b| {
                                (
                                    f64::from_ne_bytes(b[0..size_of::<f64>()].try_into().unwrap()),
                                    (
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
                                    ),
                                )
                            })
                            .collect();

                    let cs = colors_from_proto(proto_curve3.color);
                    let cs_conf = colors_from_proto(proto_curve3.conf_color);

                    p.push(PlottingPacket::Vec3ConfCurve(NamedVec3ConfCurve {
                        plot_name: plot_name.to_owned(),
                        graph_name: curve_name.to_owned(),
                        scalar_curve: Vec3ConfCurve {
                            data: pairs_as_f64.clone(),
                            style: Vec3ConfCurveStyle {
                                color: cs,
                                conf_color: cs_conf,
                            },
                            clear_cond: clear_cond_from_proto(proto_curve3.clear_cond.unwrap()),
                            bounds: bound_from_proto(proto_curve3.bounds.unwrap()),
                        },
                    }));
                }
                crate::grpc::farm_ng::core::plotting::proto::message::Variant::Rects(_) => {}
            },
            None => todo!(),
        }
    }
    p
}
