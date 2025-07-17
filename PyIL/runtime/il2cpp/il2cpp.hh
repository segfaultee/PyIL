#pragma once

#include <string>
#include <unordered_map>

#include "utils/debug.hh"

#include "assemblies.hh"
#include "state.hh"

namespace il2cpp
{
    inline uintptr_t(*domain_get)();
    inline uintptr_t(*domain_get_assemblies)(uintptr_t domain, size_t* assemblies_count);

    inline uintptr_t(*assembly_get_image)(uintptr_t assembly);

    inline uintptr_t(*image_get_assembly)(uintptr_t image);
    inline uintptr_t(*image_get_entry_point)(uintptr_t image);
    inline size_t(*image_get_class_count)(uintptr_t image);
    inline uintptr_t(*image_get_class)(uintptr_t image, size_t ind);
    inline const char*(*image_get_filename)(uintptr_t image);
    inline const char*(*image_get_name)(uintptr_t image);

    inline uintptr_t(*class_from_name)(uintptr_t image, const char* namespaze, const char* name);
    inline uintptr_t(*class_get_method_from_name)(uintptr_t image, const char* name, int params);
    inline uintptr_t(*class_get_methods)(uintptr_t klass, void** iter);
    inline uintptr_t(*class_get_fields)(uintptr_t klass, void** iter);
    inline uintptr_t(*class_get_properties)(uintptr_t klass, void** iter);
    inline uintptr_t(*class_get_parent)(uintptr_t klass);

    inline int(*method_get_param_count)(uintptr_t method);
    inline uintptr_t(*method_get_return_type)(uintptr_t method);
    inline uintptr_t(*method_get_param)(uintptr_t method, int ind);
    inline const char*(*method_get_name)(uintptr_t method);

    inline const char*(*field_get_name)(uintptr_t field);
    inline uintptr_t(*field_get_type)(uintptr_t field);

    inline const char*(*property_get_name)(uintptr_t property);
    inline uintptr_t(*property_get_get_method)(uintptr_t property);
    inline uintptr_t(*property_get_set_method)(uintptr_t property);

    inline const char*(*type_get_name)(uintptr_t type);

    inline uintptr_t(*thread_current)();
    inline uintptr_t(*thread_attach)(uintptr_t domain);

    inline uintptr_t(*runtime_invoke)(uintptr_t method, uintptr_t obj, uintptr_t params, uintptr_t exc);

    inline uintptr_t(*string_new)(const char* str);

    inline std::unordered_map<std::string, void**> _symbols = {
        {"il2cpp_domain_get", (void**)&il2cpp::domain_get},
        {"il2cpp_domain_get_assemblies", (void**)&il2cpp::domain_get_assemblies},

        {"il2cpp_assembly_get_image", (void**)&il2cpp::assembly_get_image},

        {"il2cpp_image_get_assembly", (void**)&il2cpp::image_get_assembly},
        {"il2cpp_image_get_entry_point", (void**)&il2cpp::image_get_entry_point},
        {"il2cpp_image_get_class_count", (void**)&il2cpp::image_get_class_count},
        {"il2cpp_image_get_class", (void**)&il2cpp::image_get_class},
        {"il2cpp_image_get_filename", (void**)&il2cpp::image_get_filename},
        {"il2cpp_image_get_name", (void**)&il2cpp::image_get_name},

        {"il2cpp_class_from_name", (void**)&il2cpp::class_from_name},
        {"il2cpp_class_get_method_from_name", (void**)&il2cpp::class_get_method_from_name},
        {"il2cpp_class_get_methods", (void**)&il2cpp::class_get_methods},
        {"il2cpp_class_get_fields", (void**)&il2cpp::class_get_fields},
        {"il2cpp_class_get_properties", (void**)&il2cpp::class_get_properties},
        {"il2cpp_class_get_parent", (void**)&il2cpp::class_get_parent},

        {"il2cpp_method_get_param_count", (void**)&il2cpp::method_get_param_count},
        {"il2cpp_method_get_return_type", (void**)&il2cpp::method_get_return_type},
        {"il2cpp_method_get_param", (void**)&il2cpp::method_get_param},
        {"il2cpp_method_get_name", (void**)&il2cpp::method_get_name},

        {"il2cpp_field_get_name", (void**)&il2cpp::field_get_name},
        {"il2cpp_field_get_type", (void**)&il2cpp::field_get_type},

        {"il2cpp_property_get_name", (void**)&il2cpp::property_get_name},
        {"il2cpp_property_get_get_method", (void**)&il2cpp::property_get_get_method},
        {"il2cpp_property_get_set_method", (void**)&il2cpp::property_get_set_method},

        {"il2cpp_type_get_name", (void**)&il2cpp::type_get_name},

        {"il2cpp_thread_attach", (void**)&il2cpp::thread_attach},
        {"il2cpp_thread_current", (void**)&il2cpp::thread_current},

        {"il2cpp_runtime_invoke", (void**)&il2cpp::runtime_invoke},

        {"il2cpp_string_new", (void**)&il2cpp::string_new}
    };

    bool init(const char* mod = "GameAssembly.dll", int ms_timeout = 5'000);
}