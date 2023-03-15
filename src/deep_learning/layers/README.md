# Virtual datasets

## Rationale

The main idea is to ensure each forest receives a two-dimensional dataset as input, where the first dimension represents the instances and the second one represents the features. This is hard to guarantee since datasets have shuffled dimensions and/or concatenated from a layer to the next one.

Therefore, virtua