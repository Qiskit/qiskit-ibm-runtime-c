use std::collections::HashMap;

use crate::models;
use serde::{Deserialize, Serialize};
use serde_json::Value;

#[derive(Clone, Default, Debug, PartialEq, Serialize, Deserialize)]
pub struct EstimatorV2ResultEntry {
    pub data: HashMap<String, f64>,
    pub metadata: Value,
}

#[derive(Clone, Default, Debug, PartialEq, Serialize, Deserialize)]
pub struct EstimatorV2Result {
    pub metadata: Value,
    pub results: Vec<EstimatorV2ResultEntry>,
}
