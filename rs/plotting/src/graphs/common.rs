#[derive(Clone, Debug, Default, Copy)]
pub struct Color {
    pub r: f32,
    pub g: f32,
    pub b: f32,
    pub a: f32,
}

impl Color {
    pub fn red() -> Self {
        Color {
            r: 1.0,
            g: 0.0,
            b: 0.0,
            a: 1.0,
        }
    }

    pub fn green() -> Self {
        Color {
            r: 0.0,
            g: 1.0,
            b: 0.0,
            a: 1.0,
        }
    }

    pub fn blue() -> Self {
        Color {
            r: 0.0,
            g: 0.0,
            b: 1.0,
            a: 1.0,
        }
    }

    pub fn new() -> Self {
        Color {
            r: 0.5,
            g: 0.5,
            b: 0.5,
            a: 1.0,
        }
    }
}

#[derive(Clone, Debug, Default)]
pub enum LineType {
    #[default]
    LineStrip,
    Points,
}

#[derive(Clone, Debug, Default)]
pub struct ResetPredicate {
    pub clear_x_smaller_than: Option<f64>,
}
