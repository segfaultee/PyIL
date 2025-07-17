#include <iostream>
#include <format>

#include <il2cpp/il2cpp.hh>

namespace il2cpp::debug
{
    void dump_class_methods(const char* namespaze, const char* name) { dump_class_methods(il2cpp::class_from_name(_target_asm, namespaze, name)); }
    void dump_class_properties(const char* namespaze, const char* name) { dump_class_properties(il2cpp::class_from_name(_target_asm, namespaze, name)); }
    void dump_class_fields(const char* namespaze, const char* name) { dump_class_fields(il2cpp::class_from_name(_target_asm, namespaze, name)); }
    
    void dump_class_methods(uintptr_t cls)
    {
        std::string res = "";
        
        uintptr_t method;
        void* iter = nullptr;
        while ((method = il2cpp::class_get_methods(cls, &iter)))
        {
            int param_count = il2cpp::method_get_param_count(method);

            std::string args = "";
            for (int i = 0; i < param_count; ++i)
            {
                uintptr_t param_type = il2cpp::method_get_param(method, i);
                const char* type_name = il2cpp::type_get_name(param_type);
                
                args += std::format("{}: {}", i, type_name);
                if (i + 1 != param_count)
                    args += ", ";
            }

            res += std::format(
                "{}({}) -> {}\n", 
                il2cpp::method_get_name(method), 
                args, 
                il2cpp::type_get_name(il2cpp::method_get_return_type(method))
            );
        }

        std::cout << res << '\n';
    }

    void dump_class_fields(uintptr_t cls)
    {
        std::string res = "";

        uintptr_t field;
        void* iter = nullptr;
        while ((field = il2cpp::class_get_fields(cls, &iter)))
        {
            const char* field_name = il2cpp::field_get_name(field);
            const char* field_type = il2cpp::type_get_name(il2cpp::field_get_type(field));

            res += std::format("Field: {} {}\n", field_type, field_name);
        }

        std::cout << res << '\n';
    }

    void dump_class_properties(uintptr_t cls)
    {
        std::string res = "";

        uintptr_t property;
        void* iter = nullptr;
        while ((property = il2cpp::class_get_properties(cls, &iter)))
        {
            const char* property_name = il2cpp::property_get_name(property);
            
            auto property_getter = il2cpp::property_get_get_method(property);
            auto property_setter = il2cpp::property_get_set_method(property);

            res += std::format("Property: {} (has_getter: {}, has_setter: {})\n", property_name, property_getter != 0, property_setter != 0);

            if (property_getter)
                res += std::format("  Getter: {} -> {}\n", il2cpp::method_get_name(property_getter), il2cpp::type_get_name(il2cpp::method_get_return_type(property_getter)));
            if (property_setter)
                res += std::format("  Setter: {}\n", il2cpp::method_get_name(property_setter));
        }

        std::cout << res << '\n';
    }
}