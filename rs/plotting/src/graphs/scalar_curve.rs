use crate::graphs::common::{ClearCondition, Color, CurveTrait, LineType};

use super::common::Bounds;
use std::collections::VecDeque;

#[derive(Copy, Clone, Debug)]
pub struct ScalarCurveStyle {
    pub color: Color,
    pub line_type: LineType,
}

#[derive(Clone, Debug)]
pub struct ScalarCurve {
    pub data: VecDeque<(f64, f64)>,
    pub style: ScalarCurveStyle,
    pub clear_cond: ClearCondition,
    pub bounds: Bounds,
}

impl ScalarCurve {
    pub fn new(
        data: VecDeque<(f64, f64)>,
        color: Color,
        line_type: LineType,
        bounds: Bounds,
    ) -> Self {
        ScalarCurve {
            data,
            style: ScalarCurveStyle { color, line_type },
            clear_cond: ClearCondition::new(),
            bounds,
        }
    }
}

impl CurveTrait<f64, ScalarCurveStyle> for ScalarCurve {
    fn mut_tuples(&mut self) -> &mut std::collections::VecDeque<(f64, f64)> {
        &mut self.data
    }

    fn assign_style(&mut self, style: ScalarCurveStyle) {
        self.style = style;
    }
}

#[derive(Clone, Debug)]
pub struct NamedScalarCurve {
    pub plot_name: String,
    pub graph_name: String,
    pub scalar_curve: ScalarCurve,
}
