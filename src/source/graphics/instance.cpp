#include "../../include/core/graphics/instance.h"

namespace halogen
{
    Instance::Instance() : m_vulkan_instance(VK_NULL_HANDLE), m_debug_messenger(VK_NULL_HANDLE)
    {
        /* Set application and engine name to some default values. */
        m_application_name = "Halogen Engine";
        m_engine_name = "Halogen Engine";

        create_instance();
        create_debug_messenger();
    }

    Instance::Instance(const std::string& application_name, const std::string& engine_name)
    {
        m_application_name = application_name;
        m_engine_name = engine_name;

        create_instance();
        create_debug_messenger();
    }

    VkInstance Instance::get_vulkan_instance()
    {
        return m_vulkan_instance;
    }

    Instance::~Instance()
    {
        close();
    }

    void Instance::create_instance()
    {
        if (configuration::DEBUG && !check_validation_layer_support())
        {
            debug::error("Debug mode enabled but not all validation layers found. ");
        }

        /* Optional struct for specifying additional details about the engine. */
        VkApplicationInfo application_info = {};
        application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        application_info.pNext = nullptr;
        application_info.pApplicationName = m_application_name.c_str();
        application_info.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
        application_info.pEngineName = m_engine_name.c_str();
        application_info.engineVersion = VK_MAKE_VERSION(0, 1, 0);
        application_info.apiVersion = VK_API_VERSION_1_2;

        /* Instance creation struct. */
        VkInstanceCreateInfo  instance_create_info;
        instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instance_create_info.pNext = nullptr;
        instance_create_info.flags = 0;
        instance_create_info.pApplicationInfo = &application_info;

        if (configuration::DEBUG)
        {
            instance_create_info.enabledLayerCount = static_cast<uint32_t>(configuration::VALIDATION_LAYER_COUNT);
            instance_create_info.ppEnabledLayerNames = configuration::VALIDATION_LAYERS;
        }
        else
        {
            instance_create_info.enabledLayerCount = 0;
            instance_create_info.ppEnabledLayerNames = nullptr;
        }

        std::vector<const char*> instance_extensions;
        uint32_t instance_extension_count= 0;

        if (!check_instance_extension_support(instance_extensions, instance_extension_count))
        {
            debug::error("Failed to acquire all requested instanc extensions. ");
        }

        instance_create_info.enabledExtensionCount = static_cast<uint32_t>(instance_extension_count);
        instance_create_info.ppEnabledExtensionNames = instance_extensions.data();

        VK_CHECK(vkCreateInstance(&instance_create_info, nullptr, &m_vulkan_instance), "Failed to create vulkan instance.");
        debug::log("Created Instance.");
    }


    void Instance::close()
    {
        vk_external::destroy_debug_utils_messenger_ext(m_vulkan_instance, m_debug_messenger, nullptr);
        vkDestroyInstance(m_vulkan_instance, nullptr);
    }

    void Instance::create_debug_messenger()
    {
        if (!configuration::DEBUG)
        {
            return;
        }

        VkDebugUtilsMessengerCreateInfoEXT debug_utils_messenger_create_info = {};
        construct_debug_utils_messenger_create_info(&debug_utils_messenger_create_info);

        VK_CHECK(vk_external::create_debug_utils_messenger_EXT(m_vulkan_instance, &debug_utils_messenger_create_info, nullptr, &m_debug_messenger), "Failed to create debug messenger.");

        debug::log("Created debug messenger.");
    }

    /* Helper functions to get and check required instance / extensions. */
    bool Instance::check_validation_layer_support()
    {
        if (!configuration::DEBUG)
        {
            return false;
        }

        uint32_t instance_layer_count = 0;
        std::vector<VkLayerProperties> instance_layers;

        vkEnumerateInstanceLayerProperties(&instance_layer_count, nullptr);
        instance_layers.resize(instance_layer_count);

        /* Pre check to see if all validation layers are being returned into the instance_layers vector. */
        VK_CHECK(vkEnumerateInstanceLayerProperties(&instance_layer_count, instance_layers.data()));

        /* Check if all requested validation layers are present. */
        for (const char *requested_layers : configuration::VALIDATION_LAYERS)
        {
            bool layer_found = false;
            for (const VkLayerProperties& present_layers : instance_layers)
            {
                if (strcmp(requested_layers, present_layers.layerName) == 0)
                {
                    layer_found = true;
                }
            }

            if (!layer_found)
            {
                debug::error("All instance validation layer's not found.");
            }
        }
        return true;
    }

    bool Instance::check_instance_extension_support(std::vector<const char*>& instance_extensions, uint32_t& instance_extensions_count)
    {
        Platform::get_instance_extensions(instance_extensions, instance_extensions_count);

        //Enquiring about available extension properties
        std::vector<VkExtensionProperties> available_instance_extensions;
        uint32_t available_instance_extension_count = 0;

        vkEnumerateInstanceExtensionProperties(nullptr, &available_instance_extension_count, nullptr);
        available_instance_extensions.resize(available_instance_extension_count);

        vkEnumerateInstanceExtensionProperties(nullptr, &available_instance_extension_count, available_instance_extensions.data());

        for (const char *required_extension : instance_extensions)
        {
            bool extension_found = false;
            for (const VkExtensionProperties& available_extension : available_instance_extensions)
            {
                if (strcmp(required_extension, available_extension.extensionName) == 0)
                {
                    extension_found = true;
                }
            }

            if (!extension_found)
            {
                std::cout << required_extension;
                debug::error("Required instance extensions not found. ");
                return false;
            }
        }

        return true;
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL Instance::debug_callback
    (
        VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
        VkDebugUtilsMessageTypeFlagsEXT message_type,
        const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
        void *user_data
    )
    {
        if (message_severity > VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
        {
            std::cerr << callback_data->pMessage << "[OBJECTS : ] : "
                      << callback_data->pObjects << callback_data->sType << '\n';
        }

        /* For now always return VK_FALSE since it only return true in the case of layer development. */
        return VK_FALSE;
    }

    void Instance::construct_debug_utils_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT* debug_utils_messenger_create_info)
    {
        ASSERT(debug_utils_messenger_create_info == nullptr, "Cannot construct debug utils messenger create info struct since the pointer passed is null.");

        debug_utils_messenger_create_info->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debug_utils_messenger_create_info->pNext = nullptr;
        debug_utils_messenger_create_info->flags = 0;
        debug_utils_messenger_create_info->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                                      VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
        debug_utils_messenger_create_info->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
                                      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
        debug_utils_messenger_create_info->pfnUserCallback = debug_callback;
        debug_utils_messenger_create_info->pUserData = this;
    }
}