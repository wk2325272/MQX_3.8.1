<?xml version="1.0" encoding="UTF-8"?>
<TypesAndGlobals>
  <Types>
    <Type>
      <Type>TEnumSpec</Type>
      <Name>typeRtosVersion</Name>
      <Items lines_count="1">
        <Line>3.1</Line>
      </Items>
      <Defines lines_count="1">
        <Line>3_1</Line>
      </Defines>
    </Type>
    <Type>
      <Type>TEnumSpec</Type>
      <Name>typeSerWidth_MCF</Name>
      <Items lines_count="4">
        <Line>5</Line>
        <Line>6</Line>
        <Line>7</Line>
        <Line>8</Line>
      </Items>
      <Defines lines_count="4">
        <Line>5</Line>
        <Line>6</Line>
        <Line>7</Line>
        <Line>8</Line>
      </Defines>
    </Type>
    <Type>
      <Type>TEnumSpec</Type>
      <Name>typeStopBitLength_6_7_8bits_MCF</Name>
      <Items lines_count="16">
        <Line>0.563</Line>
        <Line>0.625</Line>
        <Line>0.688</Line>
        <Line>0.750</Line>
        <Line>0.813</Line>
        <Line>0.875</Line>
        <Line>0.938</Line>
        <Line>1.000</Line>
        <Line>1.563</Line>
        <Line>1.625</Line>
        <Line>1.688</Line>
        <Line>1.750</Line>
        <Line>1.813</Line>
        <Line>1.875</Line>
        <Line>1.938</Line>
        <Line>2.000</Line>
      </Items>
      <Defines lines_count="16">
        <Line>0</Line>
        <Line>1</Line>
        <Line>2</Line>
        <Line>3</Line>
        <Line>4</Line>
        <Line>5</Line>
        <Line>6</Line>
        <Line>7</Line>
        <Line>8</Line>
        <Line>9</Line>
        <Line>10</Line>
        <Line>11</Line>
        <Line>12</Line>
        <Line>13</Line>
        <Line>14</Line>
        <Line>15</Line>
      </Defines>
    </Type>
    <Type>
      <Type>TEnumSpec</Type>
      <Name>typeChannelMode_MCF</Name>
      <Items lines_count="4">
        <Line>Normal</Line>
        <Line>Automatic echo</Line>
        <Line>Local loop-back</Line>
        <Line>Remote loop-back</Line>
      </Items>
      <Defines lines_count="4">
        <Line>normal</Line>
        <Line>autoEcho</Line>
        <Line>localLoop</Line>
        <Line>remoteLoop</Line>
      </Defines>
    </Type>
    <Type>
      <Type>TEnumSpec</Type>
      <Name>typeSCIOutputMode</Name>
      <Items lines_count="3">
        <Line>Normal</Line>
        <Line>Loop</Line>
        <Line>Single-wire</Line>
      </Items>
      <Hints lines_count="3">
        <Line>Normal transmission using TxD and RxD pins</Line>
        <Line>TxD and RxD pins are internally connected.</Line>
        <Line>Transmission using one bidirectional pin.</Line>
      </Hints>
    </Type>
    <Type>
      <Type>TEnumSpec</Type>
      <Name>typeTxDPinDirection</Name>
      <Items lines_count="2">
        <Line>input</Line>
        <Line>output</Line>
      </Items>
      <Hints lines_count="2">
        <Line>TxD pin is an input in single-wire mode</Line>
        <Line>TxD pin is an output in single-wire mode</Line>
      </Hints>
      <Defines lines_count="2">
        <Line>0</Line>
        <Line>1</Line>
      </Defines>
    </Type>
    <Type>
      <Type>TEnumSpec</Type>
      <Name>typeWakeup</Name>
      <Items lines_count="2">
        <Line>Address mark wakeup</Line>
        <Line>Idle line wakeup</Line>
      </Items>
      <Hints lines_count="2">
        <Line>a 1 in MSB position of received character wakes the receiver.</Line>
        <Line>An idle character on RxD pin wakes the receiver.</Line>
      </Hints>
    </Type>
    <Type>
      <Type>TBoolSpec</Type>
      <Name>typeInversion</Name>
      <Items lines_count="2">
        <Line>Inverted</Line>
        <Line>Not inverted</Line>
      </Items>
    </Type>
    <Type>
      <Type>TEnumSpec</Type>
      <Name>typeIdleMode</Name>
      <Items lines_count="2">
        <Line>starts after start bit</Line>
        <Line>starts after stop bit</Line>
      </Items>
      <Hints lines_count="2">
        <Line>Idle character bit count starts after start bit</Line>
        <Line>Idle character bit count starts after stop bit</Line>
      </Hints>
      <Defines lines_count="2">
        <Line>0</Line>
        <Line>1</Line>
      </Defines>
    </Type>
    <Type>
      <Type>TEnumSpec</Type>
      <Name>typeBreakLength</Name>
      <Items lines_count="2">
        <Line>10/11 bits</Line>
        <Line>13/14 bits</Line>
      </Items>
    </Type>
    <Type>
      <Type>TEnumSpec</Type>
      <Name>typeNarrowPulse</Name>
      <Items lines_count="3">
        <Line>3/16 narrow pulse</Line>
        <Line>1/16 narrow pulse</Line>
        <Line>1/32 narrow pulse</Line>
      </Items>
    </Type>
  </Types>
</TypesAndGlobals>
