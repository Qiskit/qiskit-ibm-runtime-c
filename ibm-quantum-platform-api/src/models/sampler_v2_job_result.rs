use std::collections::HashMap;

use crate::models;
use serde::{Deserialize, Serialize};
use serde_json::Value;

#[derive(Clone, Default, Debug, PartialEq, Serialize, Deserialize)]
pub struct SamplerV2ResultEntryData {
    pub samples: Vec<String>,
    pub num_bits: u32,
}

#[derive(Clone, Default, Debug, PartialEq, Serialize, Deserialize)]
pub struct SamplerV2ResultEntry {
    pub data: HashMap<String, SamplerV2ResultEntryData>,
    pub metadata: Value,
}

#[derive(Clone, Default, Debug, PartialEq, Serialize, Deserialize)]
pub struct SamplerV2Result {
    pub metadata: Value,
    pub results: Vec<SamplerV2ResultEntry>,
}
