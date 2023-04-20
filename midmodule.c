#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <time.h>
#include <stdlib.h>

static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

static void base64_encode(const unsigned char *data, size_t data_len, char *encoded_data) {
    size_t i, j;
    for (i = 0, j = 0; i < data_len;) {
        uint32_t octet_a = i < data_len ? data[i++] : 0;
        uint32_t octet_b = i < data_len ? data[i++] : 0;
        uint32_t octet_c = i < data_len ? data[i++] : 0;
        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;
        encoded_data[j++] = base64_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = base64_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = base64_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = base64_table[(triple >> 0 * 6) & 0x3F];
    }
}

static PyObject *mid_generate_mid(PyObject *self, PyObject *args) {
    unsigned char buffer[12];
    time_t seconds = time(NULL);
    buffer[0] = (seconds >> 24) & 0xFF;
    buffer[1] = (seconds >> 16) & 0xFF;
    buffer[2] = (seconds >> 8) & 0xFF;
    buffer[3] = seconds & 0xFF;
    for (int i = 4; i < 12; i++) {
        buffer[i] = rand() % 256;
    }
    char encoded_data[16];
    base64_encode(buffer, sizeof(buffer), encoded_data);
    return Py_BuildValue("s#", encoded_data, sizeof(encoded_data));
}

static PyObject *mid_get_time(PyObject *self, PyObject *args) {
    const char *mid;
    if (!PyArg_ParseTuple(args, "s", &mid)) {
        return NULL;
    }
    uint32_t seconds = ((unsigned char)mid[0] << 24)
                     | ((unsigned char)mid[1] << 16)
                     | ((unsigned char)mid[2] << 8)
                     | (unsigned char)mid[3];
    return Py_BuildValue("I", seconds);
}

static PyObject *mid_get_date_time(PyObject *self, PyObject *args) {
    const char *mid;
    if (!PyArg_ParseTuple(args, "s", &mid)) {
        return NULL;
    }
    uint32_t seconds = ((unsigned char)mid[0] << 24)
                     | ((unsigned char)mid[1] << 16)
                     | ((unsigned char)mid[2] << 8)
                     | (unsigned char)mid[3];
    PyObject *datetime_module = PyImport_ImportModule("datetime");
    if (datetime_module == NULL) {
        return NULL;
    }
    PyObject *datetime_class = PyObject_GetAttrString(datetime_module, "datetime");
    if (datetime_class == NULL) {
        Py_DECREF(datetime_module);
        return NULL;
    }
    PyObject *datetime_obj = PyObject_CallMethod(datetime_class, "fromtimestamp", "I", seconds);
    Py_DECREF(datetime_class);
    Py_DECREF(datetime_module);
    return datetime_obj;
}

static PyMethodDef MidMethods[] = {
    {"generate_mid", mid_generate_mid, METH_VARARGS, "Generate a new MID."},
    {"get_time", mid_get_time, METH_VARARGS, "Get the time from a MID."},
    {"get_date_time", mid_get_date_time, METH_VARARGS, "Get the date and time from a MID."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef midmodule = {
    PyModuleDef_HEAD_INIT,
    "mid",
    NULL,
    -1,
    MidMethods
};

PyMODINIT_FUNC PyInit_mid(void) {
    srand(time(NULL));
    return PyModule_Create(&midmodule);
}