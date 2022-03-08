file(GLOB fonts    "Fonts/*")

juce_add_binary_data(shared_resources
        HEADER_NAME SharedBinaryResources.h
        NAMESPACE SharedResources
        SOURCES
        ${fonts}
        )