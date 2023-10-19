use std::mem::size_of;

use crate::graphs::common::{Bounds, LineType, OrdinateBounds, ResetPredicate};
use crate::graphs::packets::{PlottingPacket, PlottingPackets};
use crate::graphs::scalar_curve::{NamedScalarCurve, ScalarCurve};
use crate::graphs::vec3_curve::{NamedVec3Curve, Vec3Curve};
use crate::graphs::vec3_conf_curve::{NamedVec3ConfCurve, Vec3ConfCurve};

use crate::grpc::farm_ng::core::plotting::proto;
use crate::grpc::farm_ng::core::plotting::proto::CurveResetPredicate;
use crate::grpc::proto::proto::Messages;

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

pub fn reset_predicate_from_proto(proto: CurveResetPredicate) -> ResetPredicate {
    if !proto.shall_clear {
        return ResetPredicate {
            clear_x_smaller_than: None,
        };
    }
    ResetPredicate {
        clear_x_smaller_than: Some(proto.clear_x_smaller_than),
    }
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

                    let pairs_as_f64: Vec<(f64, f64)> = proto_curve
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
                            color: color_from_proto(proto_curve.color.unwrap()),
                            curve_type: line_type_from_proto(proto_curve.line_type.unwrap()),
                            clear_x_smaller_than: reset_predicate_from_proto(
                                proto_curve.reset.unwrap(),
                            ),
                            bounds: Bounds {
                                x_bounds: OrdinateBounds {
                                    largest: 0.0,
                                    len: 100.0,
                                    data_driven: true,
                                },
                                y_bounds: OrdinateBounds {
                                    largest: 0.0,
                                    len: 100.0,
                                    data_driven: true,
                                },
                            },
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

                    let pairs_as_f64: Vec<(f64, (f64, f64, f64))> = proto_curve3
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
                            color: cs,
                            curve_type: line_type_from_proto(proto_curve3.line_type.unwrap()),
                            clear_x_smaller_than: reset_predicate_from_proto(
                                proto_curve3.reset.unwrap(),
                            ),
                            bounds: Bounds {
                                x_bounds: OrdinateBounds {
                                    largest: 0.0,
                                    len: 100.0,
                                    data_driven: true,
                                },
                                y_bounds: OrdinateBounds {
                                    largest: 0.0,
                                    len: 100.0,
                                    data_driven: true,
                                },
                            },
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
                    let pairs_as_f64: Vec<(f64, (f64, f64, f64), (f64, f64, f64))> = proto_curve3
                        .x_vec_conf_tuples
                        .unwrap()
                        .data
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

                    for (x, y, z) in pairs_as_f64.clone() {
                        v.push((x, [y.0, y.1, y.2], [z.0, z.1, z.2]));
                    }
                    let cs = colors_from_proto(proto_curve3.color);
                    let cs_conf = colors_from_proto(proto_curve3.conf_color);

                    p.push(PlottingPacket::Vec3ConfCurve(NamedVec3ConfCurve {
                        plot_name: plot_name.to_owned(),
                        graph_name: curve_name.to_owned(),
                        scalar_curve: Vec3ConfCurve {
                            data: pairs_as_f64.clone(),
                            color: cs,
                            conf_color: cs_conf,
                            curve_type: line_type_from_proto(proto_curve3.line_type.unwrap()),
                            clear_x_smaller_than: reset_predicate_from_proto(
                                proto_curve3.reset.unwrap(),
                            ),
                            bounds: Bounds {
                                x_bounds: OrdinateBounds {
                                    largest: 0.0,
                                    len: 100.0,
                                    data_driven: true,
                                },
                                y_bounds: OrdinateBounds {
                                    largest: 0.0,
                                    len: 100.0,
                                    data_driven: true,
                                },
                            },
                        },
                    }));
                }
                crate::grpc::farm_ng::core::plotting::proto::message::Variant::Rects(_) => {}
                crate::grpc::farm_ng::core::plotting::proto::message::Variant::XRange(_) => {}
                crate::grpc::farm_ng::core::plotting::proto::message::Variant::YRange(_) => {}
            },
            None => todo!(),
        }
    }
    p
}
