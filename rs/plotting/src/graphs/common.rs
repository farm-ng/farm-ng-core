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

    pub fn dark_red() -> Self {
        Color {
            r: 0.5,
            g: 0.0,
            b: 0.0,
            a: 1.0,
        }
    }

    pub fn dark_green() -> Self {
        Color {
            r: 0.0,
            g: 0.5,
            b: 0.0,
            a: 1.0,
        }
    }

    pub fn dark_blue() -> Self {
        Color {
            r: 0.0,
            g: 0.0,
            b: 0.5,
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
pub struct XCoordinateBounds {
    /// max_x is derived from graph data
    pub data_driven: bool,
    ///  largest x value / right bound of plot window
    pub max_x: f64,
    /// length of x axis, min_x = max_x - len
    pub len: f64,
}

impl XCoordinateBounds {
    pub fn from_len_and_max(len: f64, max: Option<f64>) -> Self {
        XCoordinateBounds {
            data_driven: max.is_none(),
            max_x: max.unwrap_or_default(),
            len,
        }
    }
}

#[derive(Clone, Debug, Copy)]
pub struct YCoordinateBounds {
    /// height is derived from graph data
    /// height = 0.5 * max{abs(y)}
    pub data_driven: bool,
    /// If offset is zero, then `height/2` is the top of the plot window
    /// and `-height/2` is the bottom of the plot window
    pub height: f64,
    /// is this is zero, then the y axis is centered on zero
    pub offset: f64,
}

impl YCoordinateBounds {
    pub fn symmetric_from_height(height: Option<f64>) -> Self {
        YCoordinateBounds {
            data_driven: height.is_none(),
            height: height.unwrap_or(1.0),
            offset: 0.0,
        }
    }
}

#[derive(Clone, Debug, Copy)]
pub struct Bounds {
    pub x_bounds: XCoordinateBounds,
    pub y_bounds: YCoordinateBounds,
}

#[derive(Copy, Clone, Debug, Default)]
pub enum LineType {
    #[default]
    LineStrip,
    Points,
}

#[derive(Copy, Clone, Debug)]
pub struct ClearCondition {
    pub max_x_range: f64,
}

impl ClearCondition {
    pub fn new() -> Self {
        ClearCondition { max_x_range: 0.0 }
    }

    pub fn append() -> Self {
        ClearCondition {
            max_x_range: 10000.0,
        }
    }
}

pub trait CurveTrait<DataChunk, Style> {
    fn mut_tuples(&mut self) -> &mut std::collections::VecDeque<(f64, DataChunk)>;

    fn append(
        &mut self,
        clear_cond: ClearCondition,
        mut new_tuples: std::collections::VecDeque<(f64, DataChunk)>,
        style: Style,
    ) {
        self.drain_filter(clear_cond);

        self.mut_tuples().append(&mut new_tuples);

        self.assign_style(style);
    }

    fn assign_style(&mut self, meta: Style);

    fn drain_filter(&mut self, pred: ClearCondition) {
        let max_x = self
            .mut_tuples()
            .iter()
            .fold(std::f64::MIN, |max, p| max.max(p.0));

        self.mut_tuples()
            .retain(|pair| max_x - pair.0 <= pred.max_x_range);
    }
}
