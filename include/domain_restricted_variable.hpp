#ifndef DOMAIN_RESTRICTED_VARIABLE_HPP
#define DOMAIN_RESTRICTED_VARIABLE_HPP

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#   if (defined QT_CORE_LIB || defined QT_GUI_LIB) && not defined DOMAIN_RESTRICTED_VARIABLE_NO_QSTRING
#       define STRING_TYPE QString
#   else
#       define STRING_TYPE std::string
#   endif

template<class value_type>
class DomainRestrictedVariable;

template<class value_type>
class VariableDomain;

template<class value_type>
bool operator==(
    const VariableDomain<value_type>& lhs,
    const VariableDomain<value_type>& rhs
);
template<class value_type>
bool operator!=(
    const VariableDomain<value_type>& lhs,
    const VariableDomain<value_type>& rhs
);

template<class value_type>
class VariableDomain {
    private:
    friend class DomainRestrictedVariable<value_type>;
    using storage_type = std::map<STRING_TYPE, value_type>;

    public:
    using iterator = typename storage_type::iterator;
    using const_iterator = typename storage_type::const_iterator;
    using reverse_iterator = typename storage_type::reverse_iterator;
    using const_reverse_iterator = typename storage_type::const_reverse_iterator;
    using size_type = typename storage_type::size_type;

    VariableDomain(
        std::initializer_list<std::pair<STRING_TYPE, value_type>> ilist = {}
    );
    template<class InputIt>
    VariableDomain(InputIt first, InputIt last);

    VariableDomain(const VariableDomain& other);
    VariableDomain(VariableDomain&& other) = default;

    VariableDomain& operator=(const VariableDomain& other) = delete;
    VariableDomain& operator=(VariableDomain&& other) = delete;

    ~VariableDomain() noexcept(false);

    //Iterator
    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;
    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    const_reverse_iterator crbegin() const;
    reverse_iterator rend();
    const_reverse_iterator rend() const;
    const_reverse_iterator crend() const;

    //Size
    size_type size() const;
    bool empty() const;

    //Check
    bool isName(const STRING_TYPE& name) const;
    value_type value(const STRING_TYPE& name) const;

    //Insertion
    bool addValue(const STRING_TYPE& name, const value_type& value);
    bool addValue(const STRING_TYPE& name, value_type&& value);
    template<class... Args>
    bool emplaceValue(const STRING_TYPE& name, Args&&... args);

    //Deletion
    bool removeValue(const STRING_TYPE& name);
    void removeValues(std::initializer_list<STRING_TYPE> names);

    //Replacement
    bool changeValue(const STRING_TYPE& name, const value_type& new_value);
    bool changeValue(const STRING_TYPE& name, value_type&& new_value);
    bool changeName(const STRING_TYPE& old_name, const STRING_TYPE& new_name);
    bool changeName(const STRING_TYPE& old_name, STRING_TYPE&& new_name);

    //Retrieval
    std::vector<STRING_TYPE> names() const;
    std::vector<value_type> values() const;
    std::vector<std::pair<STRING_TYPE, value_type>> valueNamePairs() const;

    friend bool operator== <value_type>(
        const VariableDomain&, const VariableDomain&
    );
    friend bool operator!= <value_type>(
        const VariableDomain&, const VariableDomain&
    );

    private:
    storage_type m_allowed_values;

    std::vector<DomainRestrictedVariable<value_type>*> m_managed_variables;

    void subscribeVariable(DomainRestrictedVariable<value_type>* ptr);
    void unsubscribeVariable(DomainRestrictedVariable<value_type>* ptr);

    void deletionNotice(value_type* to_delete);
    void replacement_notice(value_type* to_replace, value_type* replacement);
};

template<class value_type>
bool operator==(
    const DomainRestrictedVariable<value_type>& lhs,
    const DomainRestrictedVariable<value_type>& rhs
);
template<class value_type>
bool operator!=(
    const DomainRestrictedVariable<value_type>& lhs,
    const DomainRestrictedVariable<value_type>& rhs
);
template<class value_type>
bool operator<(
    const DomainRestrictedVariable<value_type>& lhs,
    const DomainRestrictedVariable<value_type>& rhs
);
template<class value_type>
bool operator>(
    const DomainRestrictedVariable<value_type>& lhs,
    const DomainRestrictedVariable<value_type>& rhs
);
template<class value_type>
bool operator<=(
    const DomainRestrictedVariable<value_type>& lhs,
    const DomainRestrictedVariable<value_type>& rhs
);
template<class value_type>
bool operator>=(
    const DomainRestrictedVariable<value_type>& lhs,
    const DomainRestrictedVariable<value_type>& rhs
);

template<class value_type>
class DomainRestrictedVariable {
    public:
    DomainRestrictedVariable(
        VariableDomain<value_type>& domain,
        const STRING_TYPE& value_name
    );
    DomainRestrictedVariable(VariableDomain<value_type>& domain);

    DomainRestrictedVariable(const DomainRestrictedVariable& other);
    DomainRestrictedVariable(DomainRestrictedVariable&& other);

    ~DomainRestrictedVariable();

    DomainRestrictedVariable& operator=(const DomainRestrictedVariable& other);
    DomainRestrictedVariable& operator=(DomainRestrictedVariable&& other);
    DomainRestrictedVariable& operator=(const STRING_TYPE& value_name);

    void clear();
    bool has_value() const;

    const value_type& value() const;

    operator const value_type&() const;

    friend bool operator== <value_type>(
        const DomainRestrictedVariable&, const DomainRestrictedVariable&
    );
    friend bool operator!= <value_type>(
        const DomainRestrictedVariable&, const DomainRestrictedVariable&
    );
    friend bool operator< <value_type>(
        const DomainRestrictedVariable&, const DomainRestrictedVariable&
    );
    friend bool operator> <value_type>(
        const DomainRestrictedVariable&, const DomainRestrictedVariable&
    );
    friend bool operator<= <value_type>(
        const DomainRestrictedVariable&, const DomainRestrictedVariable&
    );
    friend bool operator>= <value_type>(
        const DomainRestrictedVariable&, const DomainRestrictedVariable&
    );

    private:
    friend class VariableDomain<value_type>;
    std::reference_wrapper<VariableDomain<value_type>> m_domain;
    std::string m_value;

    void deletionNotice(std::string to_delete);
    void replacement_notice(std::string to_replace, std::string replacement);
};

//VariableDomain::Constructor
template<class value_type>
VariableDomain<value_type>::VariableDomain(
    std::initializer_list<std::pair<STRING_TYPE, value_type>> ilist
):
    m_allowed_values(),
    m_managed_variables()
{
    for(auto& item : ilist) {
        m_allowed_values.emplace(item.first, item.second);
    }
}
template<class value_type>
template<class InputIt>
VariableDomain<value_type>::VariableDomain(
    InputIt first, InputIt last
):
    m_allowed_values(first, last),
    m_managed_variables()
{}

template<class value_type>
VariableDomain<value_type>::VariableDomain(
    const VariableDomain& other
):
    m_allowed_values(other.m_allowed_values),
    m_managed_variables()
{}

//VariableDomain::Destructor
template<class value_type>
VariableDomain<value_type>::~VariableDomain() noexcept(false) {
    if(!m_managed_variables.empty()) {
        throw std::logic_error(
            "Cannot destroy a VariableDomain if DomainRestrictedVariable(s) depend(s) on it.\n"
        );
    }
}

//VariableDomain::Iterators
template<class value_type>
typename VariableDomain<value_type>::iterator VariableDomain<value_type>::begin() {
    return m_allowed_values.begin();
}
template<class value_type>
typename VariableDomain<value_type>::const_iterator VariableDomain<value_type>::begin() const {
    return m_allowed_values.begin();
}
template<class value_type>
typename VariableDomain<value_type>::const_iterator VariableDomain<value_type>::cbegin() const {
    return m_allowed_values.cbegin();
}
template<class value_type>
typename VariableDomain<value_type>::iterator VariableDomain<value_type>::end() {
    return m_allowed_values.end();
}
template<class value_type>
typename VariableDomain<value_type>::const_iterator VariableDomain<value_type>::end() const {
    return m_allowed_values.end();
}
template<class value_type>
typename VariableDomain<value_type>::const_iterator VariableDomain<value_type>::cend() const {
    return m_allowed_values.cend();
}
template<class value_type>
typename VariableDomain<value_type>::reverse_iterator VariableDomain<value_type>::rbegin() {
    return m_allowed_values.rbegin();
}
template<class value_type>
typename VariableDomain<value_type>::const_reverse_iterator VariableDomain<value_type>::rbegin() const {
    return m_allowed_values.rbegin();
}
template<class value_type>
typename VariableDomain<value_type>::const_reverse_iterator VariableDomain<value_type>::crbegin() const {
    return m_allowed_values.crbegin();
}
template<class value_type>
typename VariableDomain<value_type>::reverse_iterator VariableDomain<value_type>::rend() {
    return m_allowed_values.rend();
}
template<class value_type>
typename VariableDomain<value_type>::const_reverse_iterator VariableDomain<value_type>::rend() const {
    return m_allowed_values.rend();
}
template<class value_type>
typename VariableDomain<value_type>::const_reverse_iterator VariableDomain<value_type>::crend() const{
    return m_allowed_values.crend();
}

//VariableDomain::Size
template<class value_type>
typename VariableDomain<value_type>::size_type VariableDomain<value_type>::size() const {
    return m_allowed_values.size();
}

template<class value_type>
bool VariableDomain<value_type>::empty() const {
    return m_allowed_values.empty();
}

//VariableDomain::Check
template<class value_type>
bool VariableDomain<value_type>::isName(const STRING_TYPE& name) const {
    return m_allowed_values.find(name) != m_allowed_values.end();
}

template<class value_type>
value_type VariableDomain<value_type>::value(const STRING_TYPE& name) const {
    return m_allowed_values.at(name);
}

//VariableDomain::Addition
template<class value_type>
bool VariableDomain<value_type>::addValue(
    const STRING_TYPE& name, const value_type& value
) {
    if(isName(name)) {
        return false;
    }
    return m_allowed_values.emplace(name, value).second;
}
template<class value_type>
bool VariableDomain<value_type>::addValue(
    const STRING_TYPE& name, value_type&& value
) {
    if(isName(name)) {
        return false;
    }
    return m_allowed_values.insert(std::make_pair(name, std::move(value))).second;
}

template<class value_type>
template<class... Args>
bool VariableDomain<value_type>::emplaceValue(
    const STRING_TYPE& name, Args&&... args
) {
    if(isName(name)) {
        return false;
    }
    return m_allowed_values.insert(std::make_pair(name,
            value_type(std::forward<Args>(args)...))).second;
}

//VariableDomain::Deletion
template<class value_type>
bool VariableDomain<value_type>::removeValue(const STRING_TYPE& name) {
    auto it = m_allowed_values.find(name);
    if(it == m_allowed_values.end()) {
        deletionNotice(&(it->second));
        return m_allowed_values.erase(it).second;
    }
    return false;
}
template<class value_type>
void VariableDomain<value_type>::removeValues(
    std::initializer_list<STRING_TYPE> names
) {
    for(const auto& name : names) {
        removeValue(name);
    }
}

//VariableDomain::Replacement
template<class value_type>
bool VariableDomain<value_type>::changeValue(
    const STRING_TYPE& name,
    const value_type& new_value
) {
    auto it = m_allowed_values.find(name);
    if(it == m_allowed_values.end()) {
        return false;
    }
    auto ptr = &(it->second);
    it->second = new_value;
    replacement_notice(ptr, &(it->second));
    return true;
}
template<class value_type>
bool VariableDomain<value_type>::changeValue(
    const STRING_TYPE& name,
    value_type&& new_value
) {
    auto it = m_allowed_values.find(name);
    if (it == m_allowed_values.end()) {
        return false;
    }
    auto ptr = &(it->second);
    it->second = std::move(new_value);
    replacement_notice(ptr, &(it->second));
    return true;
}

//VariableDomain::Retrieval
template<class value_type>
std::vector<STRING_TYPE> VariableDomain<value_type>::names() const {
    std::vector<STRING_TYPE> temp;
    std::transform(
        m_allowed_values.begin(), m_allowed_values.end(),
        std::back_inserter(temp),
        [](const std::pair<STRING_TYPE,value_type>& kv) {return kv.first;}
    );
    return temp;
}

template<class value_type>
std::vector<value_type> VariableDomain<value_type>::values() const {
    std::vector<value_type> temp;
    std::transform(
        m_allowed_values.begin(), m_allowed_values.end(),
        std::back_inserter(temp),
        [](const std::pair<STRING_TYPE, value_type>& kv) {return kv.second;}
    );
    return temp;
}

template<class value_type>
std::vector<std::pair<STRING_TYPE, value_type>> VariableDomain<value_type>::valueNamePairs() const {
    std::vector<std::pair<STRING_TYPE, value_type>> temp;
    std::transform(
        m_allowed_values.begin(), m_allowed_values.end(),
        std::back_inserter(temp),
        [](const std::pair<STRING_TYPE, value_type>& kv) {return kv;}
    );
    return temp;
}

template<class value_type>
bool operator==(
    const VariableDomain<value_type>& lhs,
    const VariableDomain<value_type>& rhs
) {
    return &(lhs.m_allowed_values) == &(rhs.m_allowed_values);
}

template<class value_type>
bool operator!=(
    const VariableDomain<value_type>& lhs,
    const VariableDomain<value_type>& rhs
) {
    return !(lhs == rhs);
}

//VariableDomain::Private
template<class value_type>
void VariableDomain<value_type>::subscribeVariable(
    DomainRestrictedVariable<value_type>* ptr
) {
    m_managed_variables.push_back(ptr);
}

template<class value_type>
void VariableDomain<value_type>::unsubscribeVariable(
    DomainRestrictedVariable<value_type>* ptr
) {
    auto res = std::remove(m_managed_variables.begin(), m_managed_variables.end(), ptr);
    m_managed_variables.erase(res, m_managed_variables.end());
}

template<class value_type>
void VariableDomain<value_type>::deletionNotice(value_type* to_delete) {
    for(auto& var : m_managed_variables) {
        var->deletionNotice(to_delete);
    }
}

template<class value_type>
void VariableDomain<value_type>::replacement_notice(
    value_type* to_replace, value_type* replacement
) {
    for(auto& var : m_managed_variables) {
        var->replacementNotice(to_replace, replacement);
    }
}

//DomainRestrictedVariable::Constructors
template<class value_type>
DomainRestrictedVariable<value_type>::DomainRestrictedVariable(
    VariableDomain<value_type>& domain,
    const STRING_TYPE& value_name
):
    m_domain(domain),
    m_value(
        domain.m_allowed_values.find(value_name) == domain.m_allowed_values.end()
        ? throw std::invalid_argument("Tried to assign to a DomainRestrictedVariable a value not in its domain.\n")
        : value_name
    )
{
    m_domain.get().subscribeVariable(this);
}

template<class value_type>
DomainRestrictedVariable<value_type>::DomainRestrictedVariable(
    VariableDomain<value_type>& domain
):
    m_domain(domain),
    m_value(nullptr)
{
    m_domain.get().subscribeVariable(this);
}

template<class value_type>
DomainRestrictedVariable<value_type>::DomainRestrictedVariable(
    const DomainRestrictedVariable<value_type>& other
):
    m_domain(other.m_domain),
    m_value(other.m_value)
{
    m_domain.get().subscribeVariable(this);
}

template<class value_type>
DomainRestrictedVariable<value_type>::DomainRestrictedVariable(
    DomainRestrictedVariable<value_type>&& other
):
    m_domain(std::move(other.m_domain)),
    m_value(std::move(other.m_value))
{
    m_domain.get().unsubscribeVariable(&other);
    other.m_value.clear();
    m_domain.get().subscribeVariable(this);
}

//DomainRestrictedVariable::Destructor
template<class value_type>
DomainRestrictedVariable<value_type>::~DomainRestrictedVariable() {
    m_domain.get().unsubscribeVariable(this);
}

//DomainRestrictedVariable::Assignment_Operator
template<class value_type>
DomainRestrictedVariable<value_type>& DomainRestrictedVariable<value_type>::operator=(
    const DomainRestrictedVariable<value_type>& other
) {
    m_domain.get().unsibscribeVariable(this);
    m_domain = other.m_domain;
    m_value = other.m_value;
    other.m_domain.get().subscribeVariable(this);

    return *this;
}

template<class value_type>
DomainRestrictedVariable<value_type>& DomainRestrictedVariable<value_type>::operator=(
    DomainRestrictedVariable<value_type>&& other
) {
    m_domain.get().unsubscribeVariable(this);
    m_domain = std::move(other.m_domain);
    m_value = std::move(other.m_value);
    other.m_value.clear();

    return *this;
}

template<class value_type>
DomainRestrictedVariable<value_type>& DomainRestrictedVariable<value_type>::operator=(
    const STRING_TYPE& value_name
) {
    if(
        m_domain.get().m_allowed_value.end() ==
        m_domain.get().m_allowed_values.find(value_name))
    {
        throw std::invalid_argument("Tried to assign to a DomainRestrictedVariable a value not in its domain.\n");
    }
    m_value = value_name;

    return *this;
}

template<class value_type>
void DomainRestrictedVariable<value_type>::clear() {
    m_value.clear();
}

template<class value_type>
bool DomainRestrictedVariable<value_type>::has_value() const {
    return "" != m_value;
}

template<class value_type>
const value_type& DomainRestrictedVariable<value_type>::value() const {
    return m_domain.get().m_allowed_value.find(m_value)->second;
}

template<class value_type>
DomainRestrictedVariable<value_type>::operator const value_type &() const {
    return m_domain.get().m_allowed_value.find(m_value)->second;
}

template<class value_type>
void DomainRestrictedVariable<value_type>::deletionNotice(std::string to_delete) {
    if(m_value == to_delete) {
        m_value.clear();
    }
}

template<class value_type>
void DomainRestrictedVariable<value_type>::replacement_notice(
    std::string to_replace,
    std::string replacement
) {
    if(m_value == to_replace) {
        m_value = replacement;
    }
}

template<class value_type>
bool operator==(
    const DomainRestrictedVariable<value_type>& lhs,
    const DomainRestrictedVariable<value_type>& rhs
) {
    return lhs.m_domain.get() == rhs.m_domain.get() && lhs.m_value == rhs.m_value;
}
template<class value_type>
bool operator!=(
    const DomainRestrictedVariable<value_type>& lhs,
    const DomainRestrictedVariable<value_type>& rhs
) {
    return !(lhs == rhs);
}
template<class value_type>
bool operator<(
    const DomainRestrictedVariable<value_type>& lhs,
    const DomainRestrictedVariable<value_type>& rhs
) {
    return lhs.m_domain.get() == rhs.m_domain.get() && lhs.m_value < rhs.m_value;
}
template<class value_type>
bool operator>(
    const DomainRestrictedVariable<value_type>& lhs,
    const DomainRestrictedVariable<value_type>& rhs
) {
    return rhs < lhs;
}
template<class value_type>
bool operator<=(
    const DomainRestrictedVariable<value_type>& lhs,
    const DomainRestrictedVariable<value_type>& rhs
) {
    return !(lhs > rhs);
}
template<class value_type>
bool operator>=(
    const DomainRestrictedVariable<value_type>& lhs,
    const DomainRestrictedVariable<value_type>& rhs
) {
    return !(lhs < rhs);
}

#undef STRING_TYPE
#endif
