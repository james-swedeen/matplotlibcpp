# Matplotlibcpp

This is a fork of https://github.com/lava/matplotlib-cpp.
It is used for plotting while working in a [ROS 2](https://docs.ros.org/en/jazzy/index.html) environment.
It is a C++ wrapper around Python's [Matplotlib](https://matplotlib.org/).
It is designed to be as simple as possible when performance is not a major factor.

It has been modified to build within a Colcon workspace.
Additionally, [Eigen 3](https://eigen.tuxfamily.org/index.php?title=Main_Page) specializations have been written for several of the plotting functions.
To accelerate compile times of the packages this package depend on, template specializations of many of the functions included are pre-complied.

## Dependencies

Other then Ament, which can be installed with the rest of the ROS 2 distribution you're using [here](https://docs.ros.org/en/jazzy/Installation.html), the following command will install all dependencies on an Ubuntu system.

```bash
sudo apt install python3-dev python3-numpy python3-matplotlib libeigen3-dev
```

