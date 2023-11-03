use crate::graphs::common::{ClearCondition, Color, CurveTrait};

use super::common::Bounds;
use std::collections::VecDeque;

#[derive(Clone, Debug)]
pub struct Vec3ConfCurve {
    pub data: VecDeque<(f64, ((f64, f64, f64), (f64, f64, f64)))>,
    pub style: Vec3ConfCurveStyle,
    pub clear_cond: ClearCondition,
    pub bounds: Bounds,
}

#[derive(Copy, Clone, Debug)]
pub struct Vec3ConfCurveStyle {
    pub color: [Color; 3],
    pub conf_color: [Color; 3],
}

impl Vec3ConfCurve {
    pub fn new(
        data: VecDeque<(f64, ((f64, f64, f64), (f64, f64, f64)))>,
        color: [Color; 3],
        conf_color: [Color; 3],
        bounds: Bounds,
    ) -> Self {
        Vec3ConfCurve {
            data,
            style: Vec3ConfCurveStyle { color, conf_color },
            clear_cond: ClearCondition::new(),
            bounds,
        }
    }
}

impl CurveTrait<((f64, f64, f64), (f64, f64, f64)), Vec3ConfCurveStyle> for Vec3ConfCurve {
    fn mut_tuples(
        &mut self,
    ) -> &mut std::collections::VecDeque<(f64, ((f64, f64, f64), (f64, f64, f64)))> {
        &mut self.data
    }

    fn assign_style(&mut self, style: Vec3ConfCurveStyle) {
        self.style = style;
    }
}

#[derive(Clone, Debug)]
pub struct NamedVec3ConfCurve {
    pub plot_name: String,
    pub graph_name: String,
    pub scalar_curve: Vec3ConfCurve,
}
