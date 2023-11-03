use crate::graphs::common::{ClearCondition, Color, CurveTrait, LineType};

use super::common::Bounds;
use std::collections::VecDeque;

#[derive(Clone, Debug)]
pub struct Vec3Curve {
    pub data: VecDeque<(f64, (f64, f64, f64))>,
    pub style: Vec3CurveStyle,
    pub clear_cond: ClearCondition,
    pub bounds: Bounds,
}

#[derive(Copy, Clone, Debug)]
pub struct Vec3CurveStyle {
    pub color: [Color; 3],
    pub line_type: LineType,
}

impl Vec3Curve {
    pub fn new(
        data: VecDeque<(f64, (f64, f64, f64))>,
        color: [Color; 3],
        line_type: LineType,
        bounds: Bounds,
    ) -> Self {
        Vec3Curve {
            data,
            style: Vec3CurveStyle { color, line_type },
            clear_cond: ClearCondition::new(),
            bounds,
        }
    }
}

impl CurveTrait<(f64, f64, f64), Vec3CurveStyle> for Vec3Curve {
    fn mut_tuples(
        &mut self,
    ) -> &mut std::collections::VecDeque<(f64, (f64, f64, f64))> {
        &mut self.data
    }

    fn assign_style(&mut self, style: Vec3CurveStyle) {
        self.style = style;
    }
}

#[derive(Clone, Debug)]
pub struct NamedVec3Curve {
    pub plot_name: String,
    pub graph_name: String,
    pub scalar_curve: Vec3Curve,
}
