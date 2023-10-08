use crate::graphs::common::{Color, LineType, ResetPredicate};

use super::common::Bounds;

#[derive(Clone, Debug)]
pub struct Vec3Curve {
    pub data: Vec<(f64, (f64, f64, f64))>,
    pub color: [Color; 3],
    pub curve_type: LineType,
    pub clear_x_smaller_than: ResetPredicate,
    pub bounds: Bounds,
}

impl Vec3Curve {
    pub fn new(
        data: Vec<(f64, (f64, f64, f64))>,
        color: [Color; 3],
        curve_type: LineType,
        f: ResetPredicate,
        bounds: Bounds,
    ) -> Self {
        Vec3Curve {
            data,
            color,
            curve_type,
            clear_x_smaller_than: f,
            bounds,
        }
    }

    pub fn append(
        &mut self,
        mut pairs_as_f64: Vec<(f64, (f64, f64, f64))>,
        color: [Color; 3],
        curve_type: LineType,
        clear_x_smaller_than: ResetPredicate,
    ) {
        self.drain_filter(clear_x_smaller_than);

        self.data.append(&mut pairs_as_f64);
        self.color = color;
        self.curve_type = curve_type;
    }

    fn drain_filter(&mut self, pred: ResetPredicate) {
        if pred.clear_x_smaller_than.is_none() {
            return;
        }
        let mut i = 0;
        while i < self.data.len() {
            if self.data[i].0 < pred.clear_x_smaller_than.unwrap() {
                self.data.remove(i);
            } else {
                i += 1;
            }
        }
    }
}

#[derive(Clone, Debug)]
pub struct NamedVec3Curve {
    pub plot_name: String,
    pub graph_name: String,
    pub scalar_curve: Vec3Curve,
}
