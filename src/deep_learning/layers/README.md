# Virtual datasets

## Rationale

The main idea is to ensure each forest receives a two-dimensional dataset as input, where the first dimension represents the instances and the second one represents the features. This is hard to guarantee since datasets have shuffled dimensions and/or concatenated from a layer to the next one.

Therefore, virtual datasets behave like two-dimensional datasets of shape (n_instances, n_features), but are actually interfaces to the real data. Each access to the data through a virtual dataset requires a tuple, and remaps the latter to get the real data i