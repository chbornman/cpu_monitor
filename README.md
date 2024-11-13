# CPU Monitor for macOS

A lightweight, terminal-based CPU monitoring utility for macOS that provides real-time visualization of CPU core utilization.

![CPU Monitor Screenshot](./screenshots/monitor.png)

## Features

- 📊 Real-time CPU usage monitoring for each core
- 🎨 Visual progress bars with percentage indicators
- 🖥️ Clean, minimal terminal interface
- 🚀 Efficient performance with minimal system impact
- ⚡ Updates every second
- 🍎 Native macOS support (Including Apple Silicon)

## Requirements

- macOS 10.15 or later
- Xcode Command Line Tools
- GCC or Clang compiler

## Installation

1. Clone the repository:
```bash
git clone https://github.com/yourusername/cpu_monitor.git
cd cpu_monitor
```

2. Compile the source:
```bash
gcc -o cpu_monitor cpu_monitor.c -framework IOKit -framework CoreFoundation
```

3. (Optional) Move the binary to your PATH:
```bash
sudo mv cpu_monitor /usr/local/bin/
```

## Usage

Simply run the compiled binary:
```bash
./cpu_monitor
```

The monitor will display:
- Individual CPU core utilization with visual bars
- Percentage values for each core
- Auto-updating display (1-second intervals)

To exit, press `Ctrl+C`.

## Technical Details

The monitor uses the following macOS frameworks and APIs:
- IOKit for system information
- CoreFoundation for property management
- Mach kernel APIs for CPU statistics
- ANSI escape codes for terminal visualization

## Contributing

Contributions are welcome! Here are some ways you can contribute:

1. 🐛 Report bugs
2. 💡 Suggest new features
3. 📝 Improve documentation
4. 🔍 Submit pull requests

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct and the process for submitting pull requests.

## Known Issues

- GPU monitoring is currently limited due to macOS system restrictions
- Terminal window must be wide enough to display progress bars properly

## Planned Features

- [ ] GPU utilization monitoring
- [ ] Memory usage statistics
- [ ] Temperature monitoring
- [ ] Configuration file support
- [ ] Custom color schemes
- [ ] Export data to CSV

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Inspired by various system monitoring tools like htop and glances
- Thanks to the macOS developer community for documentation and examples
- Built with ❤️ for the macOS community

## Support

If you encounter any problems or have suggestions, please open an issue on the GitHub repository.

---

Made with ☕ by [Your Name]
