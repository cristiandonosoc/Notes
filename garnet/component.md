# public/lib/component/cpp

It's a fidl/directory interface for exposing information out of a component.

The idea is that an Object exposes information.

Object
  |-> hash_table<string, Property>
  |-> hash_table<string, Metric>

where:
- Property: union<string, callback>
- Metric: union<int, float, double, callback>

Callback is generating the value by passing in a callback that will do the job.
