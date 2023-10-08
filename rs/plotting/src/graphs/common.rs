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

#[derive(Clone, Debug, Copy)]
pub struct RegionF64 {
    pub min: f64,
    pub max: f64,
}
#[derive(Clone, Debug, Copy)]
pub struct OrdinateBounds {
    pub largest: f64,
    pub len: f64,
    pub data_driven: bool,
}

impl OrdinateBounds {
    pub fn from_len_and_max(len: f64, max: Option<f64>) -> Self {
        OrdinateBounds {
            largest: max.unwrap_or_default(),
            len,
            data_driven: max.is_none(),
        }
    }
}

#[derive(Clone, Debug, Copy)]
pub struct Bounds {
    pub x_bounds: OrdinateBounds,
    pub y_bounds: OrdinateBounds,
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
