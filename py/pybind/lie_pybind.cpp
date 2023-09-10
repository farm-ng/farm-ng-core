//    Copyright 2022, farm-ng inc.
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>  // Required for the operator overloads
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <sophus/lie/isometry2.h>
#include <sophus/lie/isometry3.h>
#include <sophus/lie/pose3.h>

#include <iostream>

namespace py = pybind11;
using namespace pybind11::literals;  // to bring in the `_a` literal
using sophus::Pose3F64;

// to guarantee that the array is contiguous, we need to use the buffer protocol
using py_array = py::array_t<double, py::array::c_style | py::array::forcecast>;

template <typename LieGroup>
py::class_<LieGroup> bind_liegroup(py::module_& m, char const* name) {
  py::class_<LieGroup> cls(m, name);

  cls.def(py::init([]() { return LieGroup(); }))
      .def(py::init(&LieGroup::fromRotationMatrix))
      .def(py::init(&LieGroup::fromParams))
      .def("log", [](LieGroup& self) { return self.log(); })
      .def_property("params", &LieGroup::params, &LieGroup::setParams)
      .def("inverse", [](LieGroup& self) { return self.inverse(); })
      .def_static(
          "exp", [](typename LieGroup::Tangent p) { return LieGroup::exp(p); })
      .def(py::self * py::self)
      .def(
          "__mul__",
          [](LieGroup const& a_from_b,
             typename LieGroup::Point const& point_in_b) {
            return a_from_b * point_in_b;
          });
  return cls;
}

void bind_lie(py::module_& m) {
  py::object PbRotation2F64 =
      py::module_::import("farm_ng.core.lie_pb2").attr("Rotation2F64");
  py::object PbRotation3F64 =
      py::module_::import("farm_ng.core.lie_pb2").attr("Rotation3F64");
  py::object PbQuaternionF64 =
      py::module_::import("farm_ng.core.lie_pb2").attr("QuaternionF64");
  py::object PbVec3F64 =
      py::module_::import("farm_ng.core.linalg_pb2").attr("Vec3F64");
  py::object PbVec2F64 =
      py::module_::import("farm_ng.core.linalg_pb2").attr("Vec2F64");
  py::object PbIsometry2F64 =
      py::module_::import("farm_ng.core.lie_pb2").attr("Isometry2F64");
  py::object PbIsometry3F64 =
      py::module_::import("farm_ng.core.lie_pb2").attr("Isometry3F64");
  py::object PbIsometry3F64Tangent =
      py::module_::import("farm_ng.core.lie_pb2").attr("Isometry3F64Tangent");
  py::object PbPose = py::module_::import("farm_ng.core.pose_pb2").attr("Pose");
  auto rotation3F64ToProto = [PbQuaternionF64, PbRotation3F64, PbVec3F64](
                                 sophus::Rotation3F64 const& self) {
    auto quat = self.unitQuaternion();
    return PbRotation3F64(
        "unit_quaternion"_a = PbQuaternionF64(
            "imag"_a = PbVec3F64(
                "x"_a = quat.imag()[0],
                "y"_a = quat.imag()[1],
                "z"_a = quat.imag()[2]),
            "real"_a = quat.real()));
  };

  auto rotation3F64FromProto = [](py::object proto) {
    py::object pb_quat = proto.attr("unit_quaternion");
    py::object imag = pb_quat.attr("imag");

    sophus::QuaternionF64 quat;
    quat.real() = py::cast<double>(pb_quat.attr("real"));
    quat.imag() = Eigen::Vector3d(
        py::cast<double>(imag.attr("x")),
        py::cast<double>(imag.attr("y")),
        py::cast<double>(imag.attr("z")));
    static double constexpr kEps = 1e-6;
    if (std::abs(quat.squaredNorm() - 1.0) > kEps) {
      throw py::value_error(FARM_FORMAT(
          "quaternion norm ({}) is not close to 1:\n{}",
          quat.squaredNorm(),
          quat.params().transpose()));
    }
    return sophus::Rotation3F64::fromUnitQuaternion(quat);
  };

  auto isometry3F64ToProto = [rotation3F64ToProto, PbVec3F64, PbIsometry3F64](
                                 sophus::Isometry3F64 const& self) {
    Eigen::Vector3d p = self.translation();
    return PbIsometry3F64(
        "rotation"_a = rotation3F64ToProto(self.rotation()),
        "translation"_a = PbVec3F64("x"_a = p[0], "y"_a = p[1], "z"_a = p[2]));
  };

  auto isometry3F64FromProto = [rotation3F64FromProto](py::object proto) {
    auto translation = proto.attr("translation");
    return sophus::Isometry3F64(
        Eigen::Vector3d(
            py::cast<double>(translation.attr("x")),
            py::cast<double>(translation.attr("y")),
            py::cast<double>(translation.attr("z"))),
        rotation3F64FromProto(proto.attr("rotation")));
  };

  auto Pose3F64ToProto = [isometry3F64ToProto,
                          PbVec3F64,
                          PbPose,
                          PbIsometry3F64Tangent](Pose3F64 const& self) {
    Eigen::Vector3d lv = self.tangentOfBInA().head<3>();
    Eigen::Vector3d av = self.tangentOfBInA().tail<3>();
    return PbPose(
        "a_from_b"_a = isometry3F64ToProto(self.aFromB()),
        "frame_a"_a = self.frameA(),
        "frame_b"_a = self.frameB(),
        "tangent_of_b_in_a"_a = PbIsometry3F64Tangent(
            "linear_velocity"_a =
                PbVec3F64("x"_a = lv[0], "y"_a = lv[1], "z"_a = lv[2]),
            "angular_velocity"_a =
                PbVec3F64("x"_a = av[0], "y"_a = av[1], "z"_a = av[2])));
  };

  auto Pose3F64FromProto = [isometry3F64FromProto](py::object proto) {
    Pose3F64::Tangent tangent_of_b_in_a = Pose3F64::Tangent::Zero();
    auto tangent = proto.attr("tangent_of_b_in_a");
    if (!tangent.is_none()) {
      auto linear_vel = tangent.attr("linear_velocity");
      auto angular_vel = tangent.attr("angular_velocity");

      if (!linear_vel.is_none()) {
        tangent_of_b_in_a.head<3>() = Eigen::Vector3d(
            py::cast<double>(linear_vel.attr("x")),
            py::cast<double>(linear_vel.attr("y")),
            py::cast<double>(linear_vel.attr("z")));
      }

      if (!angular_vel.is_none()) {
        tangent_of_b_in_a.tail<3>() = Eigen::Vector3d(
            py::cast<double>(angular_vel.attr("x")),
            py::cast<double>(angular_vel.attr("y")),
            py::cast<double>(angular_vel.attr("z")));
      }
    }
    return Pose3F64(
        isometry3F64FromProto(proto.attr("a_from_b")),
        py::cast<std::string>(proto.attr("frame_a")),
        py::cast<std::string>(proto.attr("frame_b")),
        tangent_of_b_in_a);
  };

  bind_liegroup<sophus::Rotation2F64>(m, "Rotation2F64")
      .def_property(
          "rotation_matrix",
          &sophus::Rotation2F64::rotationMatrix,
          [](sophus::Rotation2F64& self, Eigen::Matrix2d const& mat) {
            self = sophus::Rotation2F64::fromRotationMatrix(mat);
          })
      .def(
          "to_proto",
          [PbRotation2F64](sophus::Rotation2F64& self) {
            return PbRotation2F64("theta"_a = self.log()[0]);
          })
      .def_static("from_proto", [](py::object proto) {
        return sophus::Rotation2F64::fromAngle(
            py::cast<double>(proto.attr("theta")));
      });

  bind_liegroup<sophus::Rotation3F64>(m, "Rotation3F64")
      .def_property(
          "rotation_matrix",
          &sophus::Rotation3F64::rotationMatrix,
          [](sophus::Rotation3F64& self, Eigen::Matrix3d const& mat) {
            self = sophus::Rotation3F64::fromRotationMatrix(mat);
          })
      .def("to_proto", rotation3F64ToProto)
      .def_static("from_proto", rotation3F64FromProto)
      .def_static("Rx", sophus::Rotation3F64::fromRx)
      .def_static("Ry", sophus::Rotation3F64::fromRy)
      .def_static("Rz", sophus::Rotation3F64::fromRz);

  bind_liegroup<sophus::Isometry3F64>(m, "Isometry3F64")
      .def(
          py::init([](Eigen::Vector3d const& translation,
                      sophus::Rotation3F64 const& rotation) {
            return sophus::Isometry3F64(translation, rotation);
          }),
          py::arg("translation") = Eigen::Vector3d::Zero(),
          py::arg("rotation") = sophus::Rotation3F64())
      .def_property(
          "rotation_matrix",
          &sophus::Isometry3F64::rotationMatrix,
          [](sophus::Isometry3F64& self, Eigen::Matrix3d const& mat) {
            self.setRotation(sophus::Rotation3F64::fromRotationMatrix(mat));
          })
      .def_property(
          "rotation",
          [](sophus::Isometry3F64& self) { return self.rotation(); },
          [](sophus::Isometry3F64& self, sophus::Rotation3F64 const& x) {
            return self.setRotation(x);
          })
      .def_property(
          "translation",
          [](sophus::Isometry3F64& self) { return self.translation(); },
          [](sophus::Isometry3F64& self, Eigen::Vector3d const& x) {
            return self.translation() = x;
          })
      .def("to_proto", isometry3F64ToProto)
      .def_static("from_proto", isometry3F64FromProto)
      .def_static("Rx", sophus::Isometry3F64::fromRx)
      .def_static("Ry", sophus::Isometry3F64::fromRy)
      .def_static("Rz", sophus::Isometry3F64::fromRz);

  ;
  bind_liegroup<sophus::Isometry2F64>(m, "Isometry2F64")
      .def(py::init([](Eigen::Vector2d const& translation,
                       sophus::Rotation2F64 const& rotation) {
        return sophus::Isometry2F64(translation, rotation);
      }))
      .def(py::init([](Eigen::Vector2d const& translation, double angle) {
        return sophus::Isometry2F64(
            translation, sophus::Rotation2F64::fromAngle(angle));
      }))
      .def_property(
          "rotation_matrix",
          &sophus::Isometry2F64::rotationMatrix,
          [](sophus::Isometry2F64& self, Eigen::Matrix2d const& mat) {
            self.setRotation(sophus::Rotation2F64::fromRotationMatrix(mat));
          })
      .def_property(
          "rotation",
          [](sophus::Isometry2F64& self) { return self.rotation(); },
          [](sophus::Isometry2F64& self, sophus::Rotation2F64 const& x) {
            return self.setRotation(x);
          })
      .def_property(
          "theta",
          [](sophus::Isometry2F64& self) { return self.angle(); },
          [](sophus::Isometry2F64& self, double theta) {
            return self.setRotation(sophus::Rotation2F64::fromAngle(theta));
          })
      .def_property(
          "translation",
          [](sophus::Isometry2F64& self) { return self.translation(); },
          [](sophus::Isometry2F64& self, Eigen::Vector2d const& x) {
            return self.translation() = x;
          });

  py::class_<Pose3F64>(m, "Pose3F64")
      .def(
          py::init<
              Pose3F64::Isometry const&,
              std::string const&,
              std::string const&,
              Pose3F64::Tangent const&>(),
          py::arg("a_from_b"),
          py::arg("frame_a"),
          py::arg("frame_b"),
          py::arg("tangent_of_b_in_a") = Pose3F64::Tangent::Zero())
      .def_property(
          "frame_a",
          [](Pose3F64 const& self) { return self.frameA(); },
          [](Pose3F64& self, std::string const& frame_a) {
            self.frameA() = frame_a;
          })
      .def_property(
          "frame_b",
          [](Pose3F64 const& self) { return self.frameB(); },
          [](Pose3F64& self, std::string const& frame_b) {
            self.frameB() = frame_b;
          })
      .def_property(
          "a_from_b",
          [](Pose3F64 const& self) { return self.aFromB(); },
          [](Pose3F64& self, Pose3F64::Isometry const& a_from_b) {
            self.aFromB() = a_from_b;
          })
      .def_property_readonly(
          "b_from_a", [](Pose3F64 const& self) { return self.bFromA(); })
      .def_property(
          "tangent_of_b_in_a",
          [](Pose3F64 const& self) { return self.tangentOfBInA(); },
          [](Pose3F64& self, Pose3F64::Tangent const& tangent_of_b_in_a) {
            self.tangentOfBInA() = tangent_of_b_in_a;
          })
      .def_property(
          "rotation",
          [](Pose3F64 const& self) { return self.rotation(); },
          [](Pose3F64& self, sophus::Rotation3F64 const& x) {
            return self.setRotation(x);
          })
      .def_property(
          "translation",
          [](Pose3F64 const& self) { return self.translation(); },
          [](Pose3F64& self, Eigen::Vector3d const& x) {
            return self.translation() = x;
          })
      .def("inverse", &Pose3F64::inverse)
      .def("log", &Pose3F64::log)
      .def("evolve", &Pose3F64::evolve)
      .def(
          "__mul__",
          [](Pose3F64 const& a_from_b, Pose3F64 const& b_from_c) {
            farm_ng::Expected<Pose3F64> a_from_c = a_from_b * b_from_c;
            if (a_from_c) {
              return *a_from_c;
            }
            throw py::value_error(a_from_c.error().details[0].msg);
          })
      .def(
          "__mul__",
          [](Pose3F64 const& a_from_b, Eigen::Vector3d const& point_in_b) {
            return a_from_b.aFromB() * point_in_b;
          })
      .def_static(
          "error",
          [](Pose3F64 const& lhs_a_from_b, Pose3F64 const& rhs_a_from_b) {
            farm_ng::Expected<Pose3F64::Tangent> err =
                error(lhs_a_from_b, rhs_a_from_b);
            if (err) {
              return err->array();
            }
            throw py::value_error(err.error().details[0].msg);
          })
      .def("to_proto", Pose3F64ToProto)
      .def_static("from_proto", Pose3F64FromProto);
}
