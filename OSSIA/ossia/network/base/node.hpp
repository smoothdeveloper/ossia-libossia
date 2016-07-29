#pragma once
#include <ossia/detail/callback_container.hpp>
#include <ossia/detail/ptr_container.hpp>
#include <ossia/network/common/address_properties.hpp>

#include <string>
#include <functional>
#include <memory>

#include <ossia_export.h>
#include <nano_signal_slot.hpp>

namespace ossia
{
namespace net
{
class Device;
class address;
class Node;

class OSSIA_EXPORT Node
{
    public:
        Node() = default;
        Node(const Node&) = delete;
        Node(Node&&) = delete;
        Node& operator=(const Node&) = delete;
        Node& operator=(Node&&) = delete;

        virtual ~Node();

        virtual Device& getDevice() const = 0;
        virtual Node* getParent() const = 0;

        virtual std::string getName() const = 0;
        virtual Node & setName(std::string) = 0;

        virtual address* getAddress() const = 0;
        virtual address* createAddress(Type = Type::IMPULSE) = 0;
        virtual bool removeAddress() = 0;

        // The parent has ownership
        Node* createChild(const std::string& name);
        bool removeChild(const std::string& name);
        bool removeChild(const Node& name);
        void clearChildren();

        const std::vector<std::unique_ptr<Node>>& children() const
        { return mChildren; }

    protected:
        virtual std::unique_ptr<Node> makeChild(const std::string& name) = 0;
        virtual void removingChild(Node& node) = 0;

        std::vector<std::unique_ptr<Node>> mChildren;
};

}
}
