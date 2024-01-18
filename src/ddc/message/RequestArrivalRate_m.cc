//
// Generated file, do not edit! Created by opp_msgtool 6.0 from ddc/message/RequestArrivalRate.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "RequestArrivalRate_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

namespace inet {

class LayerNodeDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
    };
  public:
    LayerNodeDescriptor();
    virtual ~LayerNodeDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(LayerNodeDescriptor)

LayerNodeDescriptor::LayerNodeDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::LayerNode)), "")
{
    propertyNames = nullptr;
}

LayerNodeDescriptor::~LayerNodeDescriptor()
{
    delete[] propertyNames;
}

bool LayerNodeDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<LayerNode *>(obj)!=nullptr;
}

const char **LayerNodeDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = { "existingClass",  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *LayerNodeDescriptor::getProperty(const char *propertyName) const
{
    if (!strcmp(propertyName, "existingClass")) return "";
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int LayerNodeDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 0+base->getFieldCount() : 0;
}

unsigned int LayerNodeDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    return 0;
}

const char *LayerNodeDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    return nullptr;
}

int LayerNodeDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->findField(fieldName) : -1;
}

const char *LayerNodeDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    return nullptr;
}

const char **LayerNodeDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *LayerNodeDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int LayerNodeDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    LayerNode *pp = omnetpp::fromAnyPtr<LayerNode>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void LayerNodeDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    LayerNode *pp = omnetpp::fromAnyPtr<LayerNode>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'LayerNode'", field);
    }
}

const char *LayerNodeDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    LayerNode *pp = omnetpp::fromAnyPtr<LayerNode>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string LayerNodeDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    LayerNode *pp = omnetpp::fromAnyPtr<LayerNode>(object); (void)pp;
    switch (field) {
        default: return "";
    }
}

void LayerNodeDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    LayerNode *pp = omnetpp::fromAnyPtr<LayerNode>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LayerNode'", field);
    }
}

omnetpp::cValue LayerNodeDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    LayerNode *pp = omnetpp::fromAnyPtr<LayerNode>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'LayerNode' as cValue -- field index out of range?", field);
    }
}

void LayerNodeDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    LayerNode *pp = omnetpp::fromAnyPtr<LayerNode>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LayerNode'", field);
    }
}

const char *LayerNodeDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    return nullptr;
}

omnetpp::any_ptr LayerNodeDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    LayerNode *pp = omnetpp::fromAnyPtr<LayerNode>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void LayerNodeDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    LayerNode *pp = omnetpp::fromAnyPtr<LayerNode>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LayerNode'", field);
    }
}

Register_Class(RequestArrivalRate)

RequestArrivalRate::RequestArrivalRate(const char *name) : ::inet::Packet(name)
{
}

RequestArrivalRate::RequestArrivalRate(const RequestArrivalRate& other) : ::inet::Packet(other)
{
    copy(other);
}

RequestArrivalRate::~RequestArrivalRate()
{
}

RequestArrivalRate& RequestArrivalRate::operator=(const RequestArrivalRate& other)
{
    if (this == &other) return *this;
    ::inet::Packet::operator=(other);
    copy(other);
    return *this;
}

void RequestArrivalRate::copy(const RequestArrivalRate& other)
{
    this->agent_name = other.agent_name;
    this->callback_node_name = other.callback_node_name;
    this->callback_port = other.callback_port;
    this->src_layer_node = other.src_layer_node;
    this->dst_layer_node = other.dst_layer_node;
}

void RequestArrivalRate::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::Packet::parsimPack(b);
    doParsimPacking(b,this->agent_name);
    doParsimPacking(b,this->callback_node_name);
    doParsimPacking(b,this->callback_port);
    doParsimPacking(b,this->src_layer_node);
    doParsimPacking(b,this->dst_layer_node);
}

void RequestArrivalRate::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::Packet::parsimUnpack(b);
    doParsimUnpacking(b,this->agent_name);
    doParsimUnpacking(b,this->callback_node_name);
    doParsimUnpacking(b,this->callback_port);
    doParsimUnpacking(b,this->src_layer_node);
    doParsimUnpacking(b,this->dst_layer_node);
}

const char * RequestArrivalRate::getAgent_name() const
{
    return this->agent_name.c_str();
}

void RequestArrivalRate::setAgent_name(const char * agent_name)
{
    this->agent_name = agent_name;
}

const char * RequestArrivalRate::getCallback_node_name() const
{
    return this->callback_node_name.c_str();
}

void RequestArrivalRate::setCallback_node_name(const char * callback_node_name)
{
    this->callback_node_name = callback_node_name;
}

int RequestArrivalRate::getCallback_port() const
{
    return this->callback_port;
}

void RequestArrivalRate::setCallback_port(int callback_port)
{
    this->callback_port = callback_port;
}

const LayerNode& RequestArrivalRate::getSrc_layer_node() const
{
    return this->src_layer_node;
}

void RequestArrivalRate::setSrc_layer_node(const LayerNode& src_layer_node)
{
    this->src_layer_node = src_layer_node;
}

const LayerNode& RequestArrivalRate::getDst_layer_node() const
{
    return this->dst_layer_node;
}

void RequestArrivalRate::setDst_layer_node(const LayerNode& dst_layer_node)
{
    this->dst_layer_node = dst_layer_node;
}

class RequestArrivalRateDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_agent_name,
        FIELD_callback_node_name,
        FIELD_callback_port,
        FIELD_src_layer_node,
        FIELD_dst_layer_node,
    };
  public:
    RequestArrivalRateDescriptor();
    virtual ~RequestArrivalRateDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(RequestArrivalRateDescriptor)

RequestArrivalRateDescriptor::RequestArrivalRateDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::RequestArrivalRate)), "inet::Packet")
{
    propertyNames = nullptr;
}

RequestArrivalRateDescriptor::~RequestArrivalRateDescriptor()
{
    delete[] propertyNames;
}

bool RequestArrivalRateDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<RequestArrivalRate *>(obj)!=nullptr;
}

const char **RequestArrivalRateDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *RequestArrivalRateDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int RequestArrivalRateDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 5+base->getFieldCount() : 5;
}

unsigned int RequestArrivalRateDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_agent_name
        FD_ISEDITABLE,    // FIELD_callback_node_name
        FD_ISEDITABLE,    // FIELD_callback_port
        FD_ISCOMPOUND,    // FIELD_src_layer_node
        FD_ISCOMPOUND,    // FIELD_dst_layer_node
    };
    return (field >= 0 && field < 5) ? fieldTypeFlags[field] : 0;
}

const char *RequestArrivalRateDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "agent_name",
        "callback_node_name",
        "callback_port",
        "src_layer_node",
        "dst_layer_node",
    };
    return (field >= 0 && field < 5) ? fieldNames[field] : nullptr;
}

int RequestArrivalRateDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "agent_name") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "callback_node_name") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "callback_port") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "src_layer_node") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "dst_layer_node") == 0) return baseIndex + 4;
    return base ? base->findField(fieldName) : -1;
}

const char *RequestArrivalRateDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "string",    // FIELD_agent_name
        "string",    // FIELD_callback_node_name
        "int",    // FIELD_callback_port
        "inet::LayerNode",    // FIELD_src_layer_node
        "inet::LayerNode",    // FIELD_dst_layer_node
    };
    return (field >= 0 && field < 5) ? fieldTypeStrings[field] : nullptr;
}

const char **RequestArrivalRateDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *RequestArrivalRateDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int RequestArrivalRateDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    RequestArrivalRate *pp = omnetpp::fromAnyPtr<RequestArrivalRate>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void RequestArrivalRateDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    RequestArrivalRate *pp = omnetpp::fromAnyPtr<RequestArrivalRate>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'RequestArrivalRate'", field);
    }
}

const char *RequestArrivalRateDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    RequestArrivalRate *pp = omnetpp::fromAnyPtr<RequestArrivalRate>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string RequestArrivalRateDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    RequestArrivalRate *pp = omnetpp::fromAnyPtr<RequestArrivalRate>(object); (void)pp;
    switch (field) {
        case FIELD_agent_name: return oppstring2string(pp->getAgent_name());
        case FIELD_callback_node_name: return oppstring2string(pp->getCallback_node_name());
        case FIELD_callback_port: return long2string(pp->getCallback_port());
        case FIELD_src_layer_node: return "";
        case FIELD_dst_layer_node: return "";
        default: return "";
    }
}

void RequestArrivalRateDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    RequestArrivalRate *pp = omnetpp::fromAnyPtr<RequestArrivalRate>(object); (void)pp;
    switch (field) {
        case FIELD_agent_name: pp->setAgent_name((value)); break;
        case FIELD_callback_node_name: pp->setCallback_node_name((value)); break;
        case FIELD_callback_port: pp->setCallback_port(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RequestArrivalRate'", field);
    }
}

omnetpp::cValue RequestArrivalRateDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    RequestArrivalRate *pp = omnetpp::fromAnyPtr<RequestArrivalRate>(object); (void)pp;
    switch (field) {
        case FIELD_agent_name: return pp->getAgent_name();
        case FIELD_callback_node_name: return pp->getCallback_node_name();
        case FIELD_callback_port: return pp->getCallback_port();
        case FIELD_src_layer_node: return omnetpp::toAnyPtr(&pp->getSrc_layer_node()); break;
        case FIELD_dst_layer_node: return omnetpp::toAnyPtr(&pp->getDst_layer_node()); break;
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'RequestArrivalRate' as cValue -- field index out of range?", field);
    }
}

void RequestArrivalRateDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    RequestArrivalRate *pp = omnetpp::fromAnyPtr<RequestArrivalRate>(object); (void)pp;
    switch (field) {
        case FIELD_agent_name: pp->setAgent_name(value.stringValue()); break;
        case FIELD_callback_node_name: pp->setCallback_node_name(value.stringValue()); break;
        case FIELD_callback_port: pp->setCallback_port(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RequestArrivalRate'", field);
    }
}

const char *RequestArrivalRateDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        case FIELD_src_layer_node: return omnetpp::opp_typename(typeid(LayerNode));
        case FIELD_dst_layer_node: return omnetpp::opp_typename(typeid(LayerNode));
        default: return nullptr;
    };
}

omnetpp::any_ptr RequestArrivalRateDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    RequestArrivalRate *pp = omnetpp::fromAnyPtr<RequestArrivalRate>(object); (void)pp;
    switch (field) {
        case FIELD_src_layer_node: return omnetpp::toAnyPtr(&pp->getSrc_layer_node()); break;
        case FIELD_dst_layer_node: return omnetpp::toAnyPtr(&pp->getDst_layer_node()); break;
        default: return omnetpp::any_ptr(nullptr);
    }
}

void RequestArrivalRateDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    RequestArrivalRate *pp = omnetpp::fromAnyPtr<RequestArrivalRate>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RequestArrivalRate'", field);
    }
}

}  // namespace inet

namespace omnetpp {

}  // namespace omnetpp

