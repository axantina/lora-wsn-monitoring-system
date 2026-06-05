# QoS Analysis Summary

This document summarizes the QoS analysis from the LoRa-based wireless sensor network monitoring system.

## Test Environment

The system was tested in line-of-sight conditions.

## Communication Topologies

Two communication topologies were tested:

1. Point-to-point topology  
   One transmitter sends data to one receiver.

2. Dual-transmitter to single-receiver topology  
   Two transmitters send data to one receiver.

## Test Distances

The test was performed using the following distance variations:

| Distance |
|---:|
| 50 m |
| 100 m |
| 150 m |
| 200 m |

## QoS Parameters

The system performance was evaluated using:

- Packet Delivery Ratio
- Packet Loss
- Delay
- Throughput
- RSSI

## Point-to-Point Topology Result

Packet loss increased as the distance increased.

| Distance | Packet Loss | Category |
|---:|---:|---|
| 50 m | 23.88% | Medium |
| 100 m | 25.81% | Poor |
| 150 m | 29.15% | Poor |
| 200 m | 40.91% | Poor |

## Dual-Transmitter Topology Result

Packet loss in the dual-transmitter topology was higher than in the point-to-point topology.

| Topology | Packet Loss Range | Category |
|---|---:|---|
| Dual transmitter | 58%–66% | Poor |

## Delay

Delay in both topologies was more than 1 second.

Based on the TIPHON standard, this result was categorized as poor.

## Throughput

Throughput in both topologies was categorized as very good because the system transmitted more than 100 bps.

## RSSI

RSSI values changed depending on the distance between transmitter and receiver.

| RSSI | Value |
|---|---:|
| Maximum | -96 dBm |
| Minimum | -122 dBm |

## Overall Result

The overall LoRa QoS performance for both point-to-point and dual-transmitter topologies was categorized as medium based on the TIPHON index.

## Notes

The dual-transmitter topology had higher packet loss because both transmitters sent data to the same receiver. This condition may cause transmission interference and increase packet loss.
