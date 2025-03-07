//#define NO_MATPLOTLIBCPP_TEMPLATE_SPECIALIZATIONS

#include<matplotlibcpp/matplotlibcpp.hpp>

MATPLOTLIBCPP_ALL(template)

void matplotlibcpp::backend(const std::string& name)
  {
      detail::s_backend = name;
  }

bool matplotlibcpp::annotate(std::string annotation, double x, double y)
  {
      detail::_interpreter::get();

      PyObject * xy = PyTuple_New(2);
      PyObject * str = PyString_FromString(annotation.c_str());

      PyTuple_SetItem(xy,0,PyFloat_FromDouble(x));
      PyTuple_SetItem(xy,1,PyFloat_FromDouble(y));

      PyObject* kwargs = PyDict_New();
      PyDict_SetItemString(kwargs, "xy", xy);

      PyObject* args = PyTuple_New(1);
      PyTuple_SetItem(args, 0, str);

      PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_annotate, args, kwargs);

      Py_DECREF(args);
      Py_DECREF(kwargs);

      if(res) Py_DECREF(res);

      return res;
  }

  bool matplotlibcpp::subplots_adjust(const std::map<std::string, double>& keywords )
  {
      detail::_interpreter::get();

      PyObject* kwargs = PyDict_New();
      for (std::map<std::string, double>::const_iterator it =
              keywords.begin(); it != keywords.end(); ++it) {
          PyDict_SetItemString(kwargs, it->first.c_str(),
                               PyFloat_FromDouble(it->second));
      }


      PyObject* plot_args = PyTuple_New(0);

      PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_subplots_adjust, plot_args, kwargs);

      Py_DECREF(plot_args);
      Py_DECREF(kwargs);
      if(res) Py_DECREF(res);

      return res;
  }

void matplotlibcpp::colorbar(PyObject* mappable, const std::map<std::string, float>& keywords)
  {
      if (mappable == NULL)
          throw std::runtime_error("Must call colorbar with PyObject* returned from an image, contour, surface, etc.");

      detail::_interpreter::get();

      PyObject* args = PyTuple_New(1);
      PyTuple_SetItem(args, 0, mappable);

      PyObject* kwargs = PyDict_New();
      for(std::map<std::string, float>::const_iterator it = keywords.begin(); it != keywords.end(); ++it)
      {
          PyDict_SetItemString(kwargs, it->first.c_str(), PyFloat_FromDouble(it->second));
      }

      PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_colorbar, args, kwargs);
      if(!res) throw std::runtime_error("Call to colorbar() failed.");

      Py_DECREF(args);
      Py_DECREF(kwargs);
      Py_DECREF(res);
  }

long matplotlibcpp::figure(long number)
  {
      detail::_interpreter::get();

      PyObject *res;
      if (number == -1)
          res = PyObject_CallObject(detail::_interpreter::get().s_python_function_figure, detail::_interpreter::get().s_python_empty_tuple);
      else {
          assert(number > 0);

          // Make sure interpreter is initialised
          detail::_interpreter::get();

          PyObject *args = PyTuple_New(1);
          PyTuple_SetItem(args, 0, PyLong_FromLong(number));
          res = PyObject_CallObject(detail::_interpreter::get().s_python_function_figure, args);
          Py_DECREF(args);
      }

      if(!res) throw std::runtime_error("Call to figure() failed.");

      PyObject* num = PyObject_GetAttrString(res, "number");
      if (!num) throw std::runtime_error("Could not get number attribute of figure object");
      const long figureNumber = PyLong_AsLong(num);

      Py_DECREF(num);
      Py_DECREF(res);

      return figureNumber;
  }

bool matplotlibcpp::fignum_exists(long number)
  {
      detail::_interpreter::get();

      PyObject *args = PyTuple_New(1);
      PyTuple_SetItem(args, 0, PyLong_FromLong(number));
      PyObject *res = PyObject_CallObject(detail::_interpreter::get().s_python_function_fignum_exists, args);
      if(!res) throw std::runtime_error("Call to fignum_exists() failed.");

      bool ret = PyObject_IsTrue(res);
      Py_DECREF(res);
      Py_DECREF(args);

      return ret;
  }

void matplotlibcpp::figure_size(size_t w, size_t h)
  {
      detail::_interpreter::get();

      const size_t dpi = 100;
      PyObject* size = PyTuple_New(2);
      PyTuple_SetItem(size, 0, PyFloat_FromDouble((double)w / dpi));
      PyTuple_SetItem(size, 1, PyFloat_FromDouble((double)h / dpi));

      PyObject* kwargs = PyDict_New();
      PyDict_SetItemString(kwargs, "figsize", size);
      PyDict_SetItemString(kwargs, "dpi", PyLong_FromSize_t(dpi));

      PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_figure,
              detail::_interpreter::get().s_python_empty_tuple, kwargs);

      Py_DECREF(kwargs);

      if(!res) throw std::runtime_error("Call to figure_size() failed.");
      Py_DECREF(res);
  }

void matplotlibcpp::legend()
  {
      detail::_interpreter::get();

      PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_legend, detail::_interpreter::get().s_python_empty_tuple);
      if(!res) throw std::runtime_error("Call to legend() failed.");

      Py_DECREF(res);
  }

void matplotlibcpp::legend(const std::map<std::string, std::string>& keywords)
  {
    detail::_interpreter::get();

    // construct keyword args
    PyObject* kwargs = PyDict_New();
    for(std::map<std::string, std::string>::const_iterator it = keywords.begin(); it != keywords.end(); ++it)
    {
      PyDict_SetItemString(kwargs, it->first.c_str(), PyString_FromString(it->second.c_str()));
    }

    PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_legend, detail::_interpreter::get().s_python_empty_tuple, kwargs);
    if(!res) throw std::runtime_error("Call to legend() failed.");

    Py_DECREF(kwargs);
    Py_DECREF(res);
  }

void matplotlibcpp::set_aspect_equal()
  {
      // expect ratio == "equal". Leaving error handling to matplotlib.
      detail::_interpreter::get();

      PyObject* args = PyTuple_New(1);
      PyTuple_SetItem(args, 0, PyString_FromString("equal"));
      PyObject* kwargs = PyDict_New();

      PyObject *ax =
      PyObject_CallObject(detail::_interpreter::get().s_python_function_gca,
        detail::_interpreter::get().s_python_empty_tuple);
      if (!ax) throw std::runtime_error("Call to gca() failed.");
      Py_INCREF(ax);

      PyObject *set_aspect = PyObject_GetAttrString(ax, "set_aspect");
      if (!set_aspect) throw std::runtime_error("Attribute set_aspect not found.");
      Py_INCREF(set_aspect);

      PyObject *res = PyObject_Call(set_aspect, args, kwargs);
      if (!res) throw std::runtime_error("Call to set_aspect() failed.");
      Py_DECREF(set_aspect);

      Py_DECREF(ax);
      Py_DECREF(args);
      Py_DECREF(kwargs);
  }

 std::array<double, 2> matplotlibcpp::xlim()
  {
      PyObject* args = PyTuple_New(0);
      PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_xlim, args);

      if(!res) throw std::runtime_error("Call to xlim() failed.");

      Py_DECREF(res);

      PyObject* left = PyTuple_GetItem(res,0);
      PyObject* right = PyTuple_GetItem(res,1);
      return { PyFloat_AsDouble(left), PyFloat_AsDouble(right) };
  }


  std::array<double, 2> matplotlibcpp::ylim()
  {
      PyObject* args = PyTuple_New(0);
      PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_ylim, args);

      if(!res) throw std::runtime_error("Call to ylim() failed.");

      Py_DECREF(res);

      PyObject* left = PyTuple_GetItem(res,0);
      PyObject* right = PyTuple_GetItem(res,1);
      return { PyFloat_AsDouble(left), PyFloat_AsDouble(right) };
  }

void matplotlibcpp::tick_params(const std::map<std::string, std::string>& keywords, const std::string axis)
  {
    detail::_interpreter::get();

    // construct positional args
    PyObject* args;
    args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, PyString_FromString(axis.c_str()));

    // construct keyword args
    PyObject* kwargs = PyDict_New();
    for (std::map<std::string, std::string>::const_iterator it = keywords.begin(); it != keywords.end(); ++it)
    {
      PyDict_SetItemString(kwargs, it->first.c_str(), PyString_FromString(it->second.c_str()));
    }


    PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_tick_params, args, kwargs);

    Py_DECREF(args);
    Py_DECREF(kwargs);
    if (!res) throw std::runtime_error("Call to tick_params() failed");

    Py_DECREF(res);
  }

  void matplotlibcpp::subplot(long nrows, long ncols, long plot_number)
  {
      detail::_interpreter::get();

      // construct positional args
      PyObject* args = PyTuple_New(3);
      PyTuple_SetItem(args, 0, PyLong_FromLong(nrows));
      PyTuple_SetItem(args, 1, PyLong_FromLong(ncols));
      PyTuple_SetItem(args, 2, PyLong_FromLong(plot_number));

      PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_subplot, args);
      if(!res) throw std::runtime_error("Call to subplot() failed.");

      Py_DECREF(args);
      Py_DECREF(res);
  }

  void matplotlibcpp::subplot2grid(long nrows, long ncols, long rowid, long colid, long rowspan, long colspan)
  {
      detail::_interpreter::get();

      PyObject* shape = PyTuple_New(2);
      PyTuple_SetItem(shape, 0, PyLong_FromLong(nrows));
      PyTuple_SetItem(shape, 1, PyLong_FromLong(ncols));

      PyObject* loc = PyTuple_New(2);
      PyTuple_SetItem(loc, 0, PyLong_FromLong(rowid));
      PyTuple_SetItem(loc, 1, PyLong_FromLong(colid));

      PyObject* args = PyTuple_New(4);
      PyTuple_SetItem(args, 0, shape);
      PyTuple_SetItem(args, 1, loc);
      PyTuple_SetItem(args, 2, PyLong_FromLong(rowspan));
      PyTuple_SetItem(args, 3, PyLong_FromLong(colspan));

      PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_subplot2grid, args);
      if(!res) throw std::runtime_error("Call to subplot2grid() failed.");

      Py_DECREF(shape);
      Py_DECREF(loc);
      Py_DECREF(args);
      Py_DECREF(res);
  }

void matplotlibcpp::title(const std::string &titlestr, const std::map<std::string, std::string> &keywords)
  {
      detail::_interpreter::get();

      PyObject* pytitlestr = PyString_FromString(titlestr.c_str());
      PyObject* args = PyTuple_New(1);
      PyTuple_SetItem(args, 0, pytitlestr);

      PyObject* kwargs = PyDict_New();
      for (auto it = keywords.begin(); it != keywords.end(); ++it) {
          PyDict_SetItemString(kwargs, it->first.c_str(), PyUnicode_FromString(it->second.c_str()));
      }

      PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_title, args, kwargs);
      if(!res) throw std::runtime_error("Call to title() failed.");

      Py_DECREF(args);
      Py_DECREF(kwargs);
      Py_DECREF(res);
  }

  void matplotlibcpp::suptitle(const std::string &suptitlestr, const std::map<std::string, std::string> &keywords)
  {
      detail::_interpreter::get();

      PyObject* pysuptitlestr = PyString_FromString(suptitlestr.c_str());
      PyObject* args = PyTuple_New(1);
      PyTuple_SetItem(args, 0, pysuptitlestr);

      PyObject* kwargs = PyDict_New();
      for (auto it = keywords.begin(); it != keywords.end(); ++it) {
          PyDict_SetItemString(kwargs, it->first.c_str(), PyUnicode_FromString(it->second.c_str()));
      }

      PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_suptitle, args, kwargs);
      if(!res) throw std::runtime_error("Call to suptitle() failed.");

      Py_DECREF(args);
      Py_DECREF(kwargs);
      Py_DECREF(res);
  }

  void matplotlibcpp::axis(const std::string &axisstr)
  {
      detail::_interpreter::get();

      PyObject* str = PyString_FromString(axisstr.c_str());
      PyObject* args = PyTuple_New(1);
      PyTuple_SetItem(args, 0, str);

      PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_axis, args);
      if(!res) throw std::runtime_error("Call to title() failed.");

      Py_DECREF(args);
      Py_DECREF(res);
  }

void matplotlibcpp::axhline(double y, double xmin , double xmax , const std::map<std::string, std::string>& keywords )
  {
      detail::_interpreter::get();

      // construct positional args
      PyObject* args = PyTuple_New(3);
      PyTuple_SetItem(args, 0, PyFloat_FromDouble(y));
      PyTuple_SetItem(args, 1, PyFloat_FromDouble(xmin));
      PyTuple_SetItem(args, 2, PyFloat_FromDouble(xmax));

      // construct keyword args
      PyObject* kwargs = PyDict_New();
      for(std::map<std::string, std::string>::const_iterator it = keywords.begin(); it != keywords.end(); ++it)
      {
          PyDict_SetItemString(kwargs, it->first.c_str(), PyString_FromString(it->second.c_str()));
      }

      PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_axhline, args, kwargs);

      Py_DECREF(args);
      Py_DECREF(kwargs);

      if(res) Py_DECREF(res);
  }

  void matplotlibcpp::axvline(double x, double ymin , double ymax , const std::map<std::string, std::string>& keywords)
  {
      detail::_interpreter::get();

      // construct positional args
      PyObject* args = PyTuple_New(3);
      PyTuple_SetItem(args, 0, PyFloat_FromDouble(x));
      PyTuple_SetItem(args, 1, PyFloat_FromDouble(ymin));
      PyTuple_SetItem(args, 2, PyFloat_FromDouble(ymax));

      // construct keyword args
      PyObject* kwargs = PyDict_New();
      for(std::map<std::string, std::string>::const_iterator it = keywords.begin(); it != keywords.end(); ++it)
      {
          PyDict_SetItemString(kwargs, it->first.c_str(), PyString_FromString(it->second.c_str()));
      }

      PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_axvline, args, kwargs);

      Py_DECREF(args);
      Py_DECREF(kwargs);

      if(res) Py_DECREF(res);
  }

  void matplotlibcpp::axvspan(double xmin, double xmax, double ymin , double ymax , const std::map<std::string, std::string>& keywords)
  {
      // construct positional args
      PyObject* args = PyTuple_New(4);
      PyTuple_SetItem(args, 0, PyFloat_FromDouble(xmin));
      PyTuple_SetItem(args, 1, PyFloat_FromDouble(xmax));
      PyTuple_SetItem(args, 2, PyFloat_FromDouble(ymin));
      PyTuple_SetItem(args, 3, PyFloat_FromDouble(ymax));

      // construct keyword args
      PyObject* kwargs = PyDict_New();
      for (auto it = keywords.begin(); it != keywords.end(); ++it) {
        if (it->first == "linewidth" || it->first == "alpha") {
          PyDict_SetItemString(kwargs, it->first.c_str(),
            PyFloat_FromDouble(std::stod(it->second)));
        } else {
          PyDict_SetItemString(kwargs, it->first.c_str(),
            PyString_FromString(it->second.c_str()));
        }
      }

      PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_axvspan, args, kwargs);
      Py_DECREF(args);
      Py_DECREF(kwargs);

      if(res) Py_DECREF(res);
  }

  void matplotlibcpp::xlabel(const std::string &str, const std::map<std::string, std::string> &keywords )
  {
      detail::_interpreter::get();

      PyObject* pystr = PyString_FromString(str.c_str());
      PyObject* args = PyTuple_New(1);
      PyTuple_SetItem(args, 0, pystr);

      PyObject* kwargs = PyDict_New();
      for (auto it = keywords.begin(); it != keywords.end(); ++it) {
          PyDict_SetItemString(kwargs, it->first.c_str(), PyUnicode_FromString(it->second.c_str()));
      }

      PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_xlabel, args, kwargs);
      if(!res) throw std::runtime_error("Call to xlabel() failed.");

      Py_DECREF(args);
      Py_DECREF(kwargs);
      Py_DECREF(res);
  }

  void matplotlibcpp::ylabel(const std::string &str, const std::map<std::string, std::string>& keywords)
  {
      detail::_interpreter::get();

      PyObject* pystr = PyString_FromString(str.c_str());
      PyObject* args = PyTuple_New(1);
      PyTuple_SetItem(args, 0, pystr);

      PyObject* kwargs = PyDict_New();
      for (auto it = keywords.begin(); it != keywords.end(); ++it) {
          PyDict_SetItemString(kwargs, it->first.c_str(), PyUnicode_FromString(it->second.c_str()));
      }

      PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_ylabel, args, kwargs);
      if(!res) throw std::runtime_error("Call to ylabel() failed.");

      Py_DECREF(args);
      Py_DECREF(kwargs);
      Py_DECREF(res);
  }

void matplotlibcpp::set_zlabel(const std::string &str, const std::map<std::string, std::string>& keywords)
  {
      detail::_interpreter::get();

      // Same as with plot_surface: We lazily load the modules here the first time
      // this function is called because I'm not sure that we can assume "matplotlib
      // installed" implies "mpl_toolkits installed" on all platforms, and we don't
      // want to require it for people who don't need 3d plots.
      static PyObject *mpl_toolkitsmod = nullptr, *axis3dmod = nullptr;
      if (!mpl_toolkitsmod) {
          PyObject* mpl_toolkits = PyString_FromString("mpl_toolkits");
          PyObject* axis3d = PyString_FromString("mpl_toolkits.mplot3d");
          if (!mpl_toolkits || !axis3d) { throw std::runtime_error("couldnt create string"); }

          mpl_toolkitsmod = PyImport_Import(mpl_toolkits);
          Py_DECREF(mpl_toolkits);
          if (!mpl_toolkitsmod) { throw std::runtime_error("Error loading module mpl_toolkits!"); }

          axis3dmod = PyImport_Import(axis3d);
          Py_DECREF(axis3d);
          if (!axis3dmod) { throw std::runtime_error("Error loading module mpl_toolkits.mplot3d!"); }
      }

      PyObject* pystr = PyString_FromString(str.c_str());
      PyObject* args = PyTuple_New(1);
      PyTuple_SetItem(args, 0, pystr);

      PyObject* kwargs = PyDict_New();
      for (auto it = keywords.begin(); it != keywords.end(); ++it) {
          PyDict_SetItemString(kwargs, it->first.c_str(), PyUnicode_FromString(it->second.c_str()));
      }

      PyObject *ax =
      PyObject_CallObject(detail::_interpreter::get().s_python_function_gca,
        detail::_interpreter::get().s_python_empty_tuple);
      if (!ax) throw std::runtime_error("Call to gca() failed.");
      Py_INCREF(ax);

      PyObject *zlabel = PyObject_GetAttrString(ax, "set_zlabel");
      if (!zlabel) throw std::runtime_error("Attribute set_zlabel not found.");
      Py_INCREF(zlabel);

      PyObject *res = PyObject_Call(zlabel, args, kwargs);
      if (!res) throw std::runtime_error("Call to set_zlabel() failed.");
      Py_DECREF(zlabel);

      Py_DECREF(ax);
      Py_DECREF(args);
      Py_DECREF(kwargs);
      if (res) Py_DECREF(res);
  }

  void matplotlibcpp::grid(bool flag)
  {
      detail::_interpreter::get();

      PyObject* pyflag = flag ? Py_True : Py_False;
      Py_INCREF(pyflag);

      PyObject* args = PyTuple_New(1);
      PyTuple_SetItem(args, 0, pyflag);

      PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_grid, args);
      if(!res) throw std::runtime_error("Call to grid() failed.");

      Py_DECREF(args);
      Py_DECREF(res);
  }

void matplotlibcpp::show(const bool block )
  {
      detail::_interpreter::get();

      PyObject* res;
      if(block)
      {
          res = PyObject_CallObject(
                  detail::_interpreter::get().s_python_function_show,
                  detail::_interpreter::get().s_python_empty_tuple);
      }
      else
      {
          PyObject *kwargs = PyDict_New();
          PyDict_SetItemString(kwargs, "block", Py_False);
          res = PyObject_Call( detail::_interpreter::get().s_python_function_show, detail::_interpreter::get().s_python_empty_tuple, kwargs);
         Py_DECREF(kwargs);
      }


      if (!res) throw std::runtime_error("Call to show() failed.");

      Py_DECREF(res);
  }

  void matplotlibcpp::close()
  {
      detail::_interpreter::get();

      PyObject* res = PyObject_CallObject(
              detail::_interpreter::get().s_python_function_close,
              detail::_interpreter::get().s_python_empty_tuple);

      if (!res) throw std::runtime_error("Call to close() failed.");

      Py_DECREF(res);
  }

  void matplotlibcpp::xkcd() {
      detail::_interpreter::get();

      PyObject* res;
      PyObject *kwargs = PyDict_New();

      res = PyObject_Call(detail::_interpreter::get().s_python_function_xkcd,
              detail::_interpreter::get().s_python_empty_tuple, kwargs);

      Py_DECREF(kwargs);

      if (!res)
          throw std::runtime_error("Call to show() failed.");

      Py_DECREF(res);
  }

  void matplotlibcpp::draw()
  {
      detail::_interpreter::get();

      PyObject* res = PyObject_CallObject(
          detail::_interpreter::get().s_python_function_draw,
          detail::_interpreter::get().s_python_empty_tuple);

      if (!res) throw std::runtime_error("Call to draw() failed.");

      Py_DECREF(res);
  }


void matplotlibcpp::save(const std::string& filename, const int dpi)
  {
      detail::_interpreter::get();

      PyObject* pyfilename = PyString_FromString(filename.c_str());

      PyObject* args = PyTuple_New(1);
      PyTuple_SetItem(args, 0, pyfilename);

      PyObject* kwargs = PyDict_New();

      if(dpi > 0)
      {
          PyDict_SetItemString(kwargs, "dpi", PyLong_FromLong(dpi));
      }

      PyObject* res = PyObject_Call(detail::_interpreter::get().s_python_function_save, args, kwargs);
      if (!res) throw std::runtime_error("Call to save() failed.");

      Py_DECREF(args);
      Py_DECREF(kwargs);
      Py_DECREF(res);
  }

  void matplotlibcpp::rcparams(const std::map<std::string, std::string>& keywords ) {
      detail::_interpreter::get();
      PyObject* args = PyTuple_New(0);
      PyObject* kwargs = PyDict_New();
      for (auto it = keywords.begin(); it != keywords.end(); ++it) {
          if ("text.usetex" == it->first)
            PyDict_SetItemString(kwargs, it->first.c_str(), PyLong_FromLong(std::stoi(it->second.c_str())));
          else PyDict_SetItemString(kwargs, it->first.c_str(), PyString_FromString(it->second.c_str()));
      }

      PyObject * update = PyObject_GetAttrString(detail::_interpreter::get().s_python_function_rcparams, "update");
      PyObject * res = PyObject_Call(update, args, kwargs);
      if(!res) throw std::runtime_error("Call to rcParams.update() failed.");
      Py_DECREF(args);
      Py_DECREF(kwargs);
      Py_DECREF(update);
      Py_DECREF(res);
  }

  void matplotlibcpp::clf() {
      detail::_interpreter::get();

      PyObject *res = PyObject_CallObject(
          detail::_interpreter::get().s_python_function_clf,
          detail::_interpreter::get().s_python_empty_tuple);

      if (!res) throw std::runtime_error("Call to clf() failed.");

      Py_DECREF(res);
  }

  void matplotlibcpp::cla() {
      detail::_interpreter::get();

      PyObject* res = PyObject_CallObject(detail::_interpreter::get().s_python_function_cla,
                                          detail::_interpreter::get().s_python_empty_tuple);

      if (!res)
          throw std::runtime_error("Call to cla() failed.");

      Py_DECREF(res);
  }

void matplotlibcpp::ion() {
      detail::_interpreter::get();

      PyObject *res = PyObject_CallObject(
          detail::_interpreter::get().s_python_function_ion,
          detail::_interpreter::get().s_python_empty_tuple);

      if (!res) throw std::runtime_error("Call to ion() failed.");

      Py_DECREF(res);
  }

  std::vector<std::array<double, 2>> matplotlibcpp::ginput(const int numClicks , const std::map<std::string, std::string>& keywords)
  {
      detail::_interpreter::get();

      PyObject *args = PyTuple_New(1);
      PyTuple_SetItem(args, 0, PyLong_FromLong(numClicks));

      // construct keyword args
      PyObject* kwargs = PyDict_New();
      for(std::map<std::string, std::string>::const_iterator it = keywords.begin(); it != keywords.end(); ++it)
      {
          PyDict_SetItemString(kwargs, it->first.c_str(), PyUnicode_FromString(it->second.c_str()));
      }

      PyObject* res = PyObject_Call(
          detail::_interpreter::get().s_python_function_ginput, args, kwargs);

      Py_DECREF(kwargs);
      Py_DECREF(args);
      if (!res) throw std::runtime_error("Call to ginput() failed.");

      const size_t len = PyList_Size(res);
      std::vector<std::array<double, 2>> out;
      out.reserve(len);
      for (size_t i = 0; i < len; i++) {
          PyObject *current = PyList_GetItem(res, i);
          std::array<double, 2> position;
          position[0] = PyFloat_AsDouble(PyTuple_GetItem(current, 0));
          position[1] = PyFloat_AsDouble(PyTuple_GetItem(current, 1));
          out.push_back(position);
      }
      Py_DECREF(res);

      return out;
  }

  // Actually, is there any reason not to call this automatically for every plot?
  void matplotlibcpp::tight_layout() {
      detail::_interpreter::get();

      PyObject *res = PyObject_CallObject(
          detail::_interpreter::get().s_python_function_tight_layout,
          detail::_interpreter::get().s_python_empty_tuple);

      if (!res) throw std::runtime_error("Call to tight_layout() failed.");

      Py_DECREF(res);
  }
