# ATSimu

**A network-scale microscopic traffic simulator**

[![Status: historical software](https://img.shields.io/badge/status-historical%20software-6b7280)](#project-status)
[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

ATSimu (originally **A Traffic Simulation**, or **ATS**) is a microscopic traffic simulation package created by [Zhengbing He](https://www.gotrafficgo.com). It was developed in Visual C++ between 2006 and 2011 and is preserved here as an early open-source research software project.

![Sample ATSimu Road Network](src/Samples/Design%20files/Sample%2820100626%20Network%29.jpg)
![Zoom-in Intersection](src/Samples/Design%20files/Sample_Intersection.png)

## Highlights

- XML-based road-network and traffic-demand input
- Microscopic vehicle movement on multi-lane links
- Gipps and velocity-dependent randomization car-following models
- Lane-changing and intersection-conflict logic
- Fixed and time-varying origin–destination demand
- Distance- and time-based shortest-path routing
- Traffic detectors, signal-control interfaces, trajectories, and macroscopic fundamental diagram output
- A day-to-day extension in which commuters repeatedly travel between fixed origin–destination pairs and adapt their routes

## Project status

This repository is a **historical software archive**. The code is provided for research reference, reproducibility, and education; it is not presented as a currently maintained production simulator.

The latest source snapshot is the day-to-day version released on May 18, 2011. It targets 32-bit Windows and Microsoft Visual C++ 2008 with MFC. The imported code has been kept substantially unchanged, apart from omitting generated Visual Studio user files and caches. It has not been rebuilt or behaviorally validated on current toolchains.

## Watch the simulation

- [YouTube](https://youtu.be/xm81spwfknk)
- [Bilibili](https://www.bilibili.com/video/BV14M4y1j7M7?t=1.8)
- [Project page](https://www.gotrafficgo.com/project/animation-microscopic-simulation/)

## Repository layout

- [`src/`](src/) — latest day-to-day source snapshot and sample networks
- [`src/traffic.sln`](src/traffic.sln) — original Visual Studio solution
- [`CITATION.cff`](CITATION.cff) — software citation metadata
- Git tag [`v2011.04.15`](https://github.com/gotrafficgo/ATSimu/tree/v2011.04.15) — baseline ATS source release
- Git tag [`v2011.05.18`](https://github.com/gotrafficgo/ATSimu/tree/v2011.05.18) — day-to-day source release

## Building the historical code

The original project uses Visual Studio 2008 project format (`.vcproj`), MFC, Win32, and MSXML.

1. Use a Windows environment with Visual C++ and MFC support.
2. Open [`src/traffic.sln`](src/traffic.sln).
3. Select a Win32 configuration and build the `traffic` project.
4. Start ATSimu, load one of the XML networks under [`src/Samples/`](src/Samples/), and choose **Run**.

Modern Visual Studio versions may offer to upgrade the project. Such upgraded builds are not currently verified.

## Releases and provenance

The Git history preserves the two principal SourceForge source releases as separate commits and tags. Original ZIP files and the historical WMV demo are attached to the corresponding GitHub releases.

The original website and downloads remain available as a permanent provenance record:

- [ATSimu website on SourceForge](https://atsimu.sourceforge.net/index.html)
- [Original SourceForge downloads](https://sourceforge.net/projects/atsimu/files/)

## Citation

If this archive supports your research, cite the software metadata in [`CITATION.cff`](CITATION.cff). GitHub also exposes it through **Cite this repository**.

## License

ATSimu is released under the [MIT License](LICENSE). Historical attribution embedded in the archived source is retained as part of the original record.
