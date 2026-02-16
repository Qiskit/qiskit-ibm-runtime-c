// This code is part of Qiskit.
//
// (C) Copyright IBM 2026
//
// This code is licensed under the Apache License, Version 2.0. You may
// obtain a copy of this license in the LICENSE.txt file in the root directory
// of this source tree or at http://www.apache.org/licenses/LICENSE-2.0.
//
// Any modifications or derivative works of this code must retain this
// copyright notice, and modified files need to carry a notice indicating
// that they have been altered from the originals.

use foldhash::fast::RandomState;
use indexmap::IndexMap;

pub struct CountsHistogram(IndexMap<String, u64, RandomState>);

impl CountsHistogram {
    pub fn from_samples(samples: &[String]) -> Self {
        let mut out = CountsHistogram(IndexMap::with_capacity_and_hasher(
            samples.len(),
            RandomState::default(),
        ));
        for sample in samples {
            match out.0.get_mut(sample) {
                Some(val) => *val += 1,
                None => {
                    out.0.insert(sample.clone(), 1);
                }
            };
        }
        out
    }

    pub fn most_frequent(&self) -> &str {
        self.0
            .iter()
            .max_by_key(|(_idx, val)| *val)
            .expect("There are no samples in the counts histogram.")
            .0
            .as_str()
    }

    pub fn least_frequent(&self) -> &str {
        self.0
            .iter()
            .min_by_key(|(_idx, val)| *val)
            .expect("There are no samples in the counts histogram.")
            .0
            .as_str()
    }

    pub fn sort_by_frequency(&mut self, most_frequent_first: bool) {
        self.0.sort_by_key(|_key, val| *val);
        if most_frequent_first {
            self.0.reverse();
        }
    }

    pub fn len(&self) -> usize {
        self.0.len()
    }

    pub fn get_item(&self, index: usize) -> Option<(&str, u64)> {
        self.0
            .get_index(index)
            .map(|(sample, count)| (sample.as_str(), *count))
    }

    pub fn display(&self) {
        for (key, val) in self.0.iter() {
            println!("{}: {}", key, val);
        }
    }
}
