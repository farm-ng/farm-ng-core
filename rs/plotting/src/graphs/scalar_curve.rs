use crate::graphs::common::Color;

#[derive(Clone, Debug, Default)]
pub struct ScalarCurve {
    pub data: Vec<(f64, f64)>,
    pub color: Color,
    pub curve_type: String,
}

impl ScalarCurve {
    pub fn new(data: Vec<(f64, f64)>, color: Color, curve_type: String) -> Self {
        ScalarCurve {
            data,
            color,
            curve_type,
        }
    }

    pub fn append(
        &mut self,
        mut pairs_as_f64: Vec<(f64, f64)>,
        color: Color,
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

#[derive(Clone, Debug, Default)]
pub struct NamedScalarCurve {
    pub plot_name: String,
    pub graph_name: String,
    pub scalar_curve: ScalarCurve,
}
