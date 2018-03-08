#ifndef RESOURCE_HOLDER_H
#define RESOURCE_HOLDER_H

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>

template<typename Resource, typename Identifier>
class ResourceHolder {
public:
    void load(Identifier id, const std::string &filename) {
        // Create and load resource
        std::unique_ptr<Resource> resource(new Resource());
        if (!resource->loadFromFile(filename))
            throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

        // If loading successful, insert resource to map
        insertResource(id, std::move(resource));
    }

    template<typename Parameter>
    void load(Identifier id, const std::string &filename, const Parameter &secondParam) {
        // Create and load resource
        std::unique_ptr<Resource> resource(new Resource());
        if (!resource->loadFromFile(filename, secondParam))
            throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

        // If loading successful, insert resource to map
        insertResource(id, std::move(resource));
    }

    Resource &get(Identifier id) {
        auto found = mResourceMap.find(id);
        assert(found != mResourceMap.end());

        return *found->second;
    }
    const Resource &get(Identifier id) const {
        auto found = mResourceMap.find(id);
        assert(found != mResourceMap.end());

        return *found->second;
    }

private:
    void insertResource(Identifier id, std::unique_ptr<Resource> resource) {
        // Insert and check success
        auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));
        assert(inserted.second);
    }

private:
    std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};

#endif //RESOURCE_HOLDER_H
