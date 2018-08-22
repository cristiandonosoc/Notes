# public/lib/component/cpp

It's a fidl/directory interface for exposing information out of a component.

Concepts
- Property: union<string, callback>
- Metric: union<int, float, double, callback>

Callback is generating the value by passing in a callback that will do the job.

## Object

Object
  |-> hash_table<string, Property>
  |-> hash_table<string, Metric>
  |-> hash_table<string, RefPtr<Object>>    // Children
  |-> LazyChildrenCallback

The idea is that an Object exposes information.
On an object, you can set a reference to a child. The children are keyed by
their name. If it already exist, it will swap the ref pointers.

The LazyChildrenCallback is called on read time to populate more objects in
addition to the ones already added to the Object.

Object exposes as fidl and as special files within a particular directory:

- .channel: Exposes the file as a fidl services that use

## ObjectDir

Concepts:
- ObjectPath: Sub-path to a children (list of strings).
- PropName: For some reason

Wrapper over Object. Suports high-level operations: Tree Traversal, Property Setters?
Will create the tree of nested objects.

## ExposedObject

ExposedObject manages parent lifecycle of ObjectDirs, removing itself from parents
on destruction.


