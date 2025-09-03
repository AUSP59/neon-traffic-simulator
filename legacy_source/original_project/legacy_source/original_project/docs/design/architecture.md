# Architecture

```mermaid
flowchart LR
  CLI[CLI] --> Sim[Simulator]
  Sim --> Net[RoadNetwork]
  Sim --> TL[TrafficLightController]
  Sim --> Veh[Vehicles]
  Net --> CSV[CSV Parsers]
  Sim --> Metrics[(Metrics)]
```
