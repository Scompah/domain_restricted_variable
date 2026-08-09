#ifndef DOMAIN_RESTRICTED_VARIABLE_HPP
#define DOMAIN_RESTRICTED_VARIABLE_HPP

#include <functional>
#include <initializer_list>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>

template<class value_type, class Compare = std::less<value_type>>
class DomainRestrictedVariable;

template<class value_type, class Compare = std::less<value_type>>
class VariableDomain {
    friend class DomainRestrictedVariable<value_type, Compare>;
    using storage_type = std::set<value_type, Compare>;

    public:
    using const_iterator = typename storage_type::const_iterator;
    using const_reverse_iterator = typename storage_type::const_reverse_iterator;

    VariableDomain(
        std::initializer_list<value_type> ilist = {},
        const Compare& comp = Compare()
    );
    explicit VariableDomain(
        const Compare& comp
    );
    template<class InputIt>
    VariableDomain(
        InputIt first, InputIt last,
        const Compare& comp = Compare()
    );

    VariableDomain(const VariableDomain& other) = delete;
    VariableDomain(VariableDomain&& other) = default;

    VariableDomain& operator=(const VariableDomain& other) = delete;
    VariableDomain& operator=(VariableDomain&& other) = delete;

    ~VariableDomain() noexcept(false);

    //Iterator
    const_iterator begin() const;
    const_iterator cbegin() const;
    const_iterator end() const;
    const_iterator cend() const;
    const_reverse_iterator rbegin() const;
    const_reverse_iterator crbegin() const;
    const_reverse_iterator rend() const;
    const_reverse_iterator crend() const;

    //Check
    bool isAllowedValue(const value_type& value) const;
#if __cplusplus >= 201402L
    template<class K>
    bool isAllowedValue(K&& x) const;
#endif

    //Addition
    bool addAllowedValue(const value_type& value);
    bool addAllowedValue(value_type&& value);
    template<class InputIt>
    void addAllowedValuesRange(InputIt first, InputIt last);
    void addAllowedValues(std::initializer_list<value_type> ilist);

    template<class... Args>
    bool emplaceAllowedValue(Args&&... args);

    //Removal
    bool removeAllowedValue(const value_type& value);
    void removeAllowedValues(std::initializer_list<value_type> ilist);

    //Replacement
    bool replaceAllowedValue(
        const value_type& to_replace,
        const value_type& replacement
    );
    bool replaceAllowedValue(
        const value_type& to_replace,
        value_type&& value
    );

    //Retrieval
    std::vector<value_type> allowedValues() const;

    private:
    storage_type m_allowed_values;

    std::set<
        DomainRestrictedVariable<value_type, Compare>*> m_managed_variables;

    void subscribeVariable(
        const DomainRestrictedVariable<value_type, Compare>* const ptr);
    void unsubscribeVariable(
        const DomainRestrictedVariable<value_type, Compare>* const ptr);

    void deletionNotice(value_type* to_delete);
    void replacementNotice(value_type* to_replace, value_type* replacement);
};

template<class value_type, class Compare>
class DomainRestrictedVariable {
    public:
    DomainRestrictedVariable(
        VariableDomain<value_type, Compare>& domain,
        const value_type& value
    );
    DomainRestrictedVariable(
        VariableDomain<value_type, Compare>& domain
    );

    DomainRestrictedVariable(const DomainRestrictedVariable& other);
    DomainRestrictedVariable(DomainRestrictedVariable&& other);

    ~DomainRestrictedVariable();

    DomainRestrictedVariable& operator=(const DomainRestrictedVariable& other);
    DomainRestrictedVariable& operator=(DomainRestrictedVariable&& other);
    DomainRestrictedVariable& operator=(const value_type& value);

    void clear();
    bool has_value() const;

    //WARNING:  If the variable is uninitialized or cleared
    //          this method has undefined behaviour
    operator value_type();

    template<class value_type, class Compare>
    friend bool operator==(
        const DomainRestrictedVariable<value_type, Compare>& lhs,
        const DomainRestrictedVariable<value_type, Compare>& rhs
    );

    template<class value_type, class Compare>
    friend bool operator!=(
        const DomainRestrictedVariable<value_type, Compare>& lhs,
        const DomainRestrictedVariable<value_type, Compare>& rhs
    );

    template<class value_type, class Compare>
    friend bool operator<(
        const DomainRestrictedVariable<value_type, Compare>& lhs,
        const DomainRestrictedVariable<value_type, Compare>& rhs
    );

    template<class value_type, class Compare>
    friend bool operator>(
        const DomainRestrictedVariable<value_type, Compare>& lhs,
        const DomainRestrictedVariable<value_type, Compare>& rhs
    );

    template<class value_type, class Compare>
    friend bool std::operator<=(
        const DomainRestrictedVariable<value_type, Compare>& lhs,
        const DomainRestrictedVariable<value_type, Compare>& rhs
    );

    template<class value_type, class Compare>
    friend bool std::operator>=(
        const DomainRestrictedVariable<value_type, Compare>& lhs,
        const DomainRestrictedVariable<value_type, Compare>& rhs
    );

    private:
    std::reference_wrapper<VariableDomain<value_type, Compare>> m_domain;
    value_type* m_value;

    void deletionNotice(value_type* to_delete);
    void replacementNotice(value_type* to_replace, value_type* replacement);
};


template<class value_type, class Compare>
VariableDomain<value_type, Compare>::VariableDomain(
    std::initializer_list<value_type> ilist,
    const Compare& comp
): m_allowed_values(ilist, comp), m_managed_variables() {}

template<class value_type, class Compare>
VariableDomain<value_type, Compare>::VariableDomain(
    const Compare& comp
): m_allowed_values(comp), m_managed_variables() {}

template<class value_type, class Compare>
template<class InputIt>
VariableDomain<value_type, Compare>::VariableDomain(
    InputIt first, InputIt last,
    const Compare& comp
): m_allowed_values(first, last, comp), m_managed_variables() {}

template<class value_type, class Compare>
VariableDomain<value_type, Compare>::~VariableDomain() noexcept(false) {
    if(!m_managed_variables.empty()) {
        throw std::logic_error(
            "Cannot destroy a VariableDomain if DomainRestrictedVariable(s) depend on it.");
    }
}

template<class value_type, class Compare>
typename VariableDomain<value_type, Compare>::const_iterator
    VariableDomain<value_type, Compare>::begin() const
{
    return m_allowed_values.begin();
}

template<class value_type, class Compare>
typename VariableDomain<value_type, Compare>::const_iterator
    VariableDomain<value_type, Compare>::cbegin() const
{
    return m_allowed_values.cbegin();
}

template<class value_type, class Compare>
typename VariableDomain<value_type, Compare>::const_iterator
    VariableDomain<value_type, Compare>::end() const
{
    return m_allowed_values.end();
}

template<class value_type, class Compare>
typename VariableDomain<value_type, Compare>::const_iterator
    VariableDomain<value_type, Compare>::cend() const
{
    return m_allowed_values.cend();
}

template<class value_type, class Compare>
typename VariableDomain<value_type, Compare>::const_reverse_iterator
    VariableDomain<value_type, Compare>::rbegin() const
{
    return m_allowed_values.rbegin();
}

template<class value_type, class Compare>
typename VariableDomain<value_type, Compare>::const_reverse_iterator
    VariableDomain<value_type, Compare>::crbegin() const
{
    return m_allowed_values.crbegin();
}

template<class value_type, class Compare>
typename VariableDomain<value_type, Compare>::const_reverse_iterator
    VariableDomain<value_type, Compare>::rend() const
{
    return m_allowed_values.rend();
}

template<class value_type, class Compare>
typename VariableDomain<value_type, Compare>::const_reverse_iterator
    VariableDomain<value_type, Compare>::crend() const
{
    return m_allowed_values.crend();
}

template<class value_type, class Compare>
bool VariableDomain<value_type, Compare>::isAllowedValue(
    const value_type& value
) const {
    return m_allowed_values.find(value) != m_allowed_values.end();
}

#if __cplusplus >= 201402L
template<class value_type, class Compare>
template<class K>
bool VariableDomain<value_type, Compare>::isAllowedValue(
    K&& x
) const {
    return m_allowed_values.find(x) != m_allowed_values.end();
}
#endif

template<class value_type, class Compare>
bool VariableDomain<value_type, Compare>::addAllowedValue(
    const value_type& value
) {
    return m_allowed_values.insert(value).second;
}

template<class value_type, class Compare>
bool VariableDomain<value_type, Compare>::addAllowedValue(
    value_type&& value
) {
    return m_allowed_values.insert(std::forward(value)).second;
}

template<class value_type, class Compare>
template<class InputIt>
void VariableDomain<value_type, Compare>::addAllowedValuesRange(
    InputIt first, InputIt last
) {
    m_allowed_values.insert(first, last);
}

template<class value_type, class Compare>
void VariableDomain<value_type, Compare>::addAllowedValues(
    std::initializer_list<value_type> ilist
) {
    m_allowed_values.insert(ilist);
}

template<class value_type, class Compare>
template<class... Args>
bool VariableDomain<value_type, Compare>::emplaceAllowedValue(
    Args&&... args
) {
    return m_allowed_values.emplace(std::forward(args)...);
}

template<class value_type, class Compare>
bool VariableDomain<value_type, Compare>::removeAllowedValue(
    const value_type& value
) {
    auto iter = m_allowed_values.find(value);
    if(value == m_allowed_values.end()) {
        return false;
    }

    deletionNotice(iter);
    m_allowed_values.erase(iter);
    return true;
}

template<class value_type, class Compare>
void VariableDomain<value_type, Compare>::removeAllowedValues(
    std::initializer_list<value_type> ilist
) {
    for(auto& value : ilist) {
        removeAllowedValue(value);
    }
}

template<class value_type, class Compare>
bool VariableDomain<value_type, Compare>::replaceAllowedValue(
    const value_type& to_replace,
    const value_type& replacement
) {
    auto iter = m_allowed_values.find(to_replace);
    if(iter == m_allowed_values.end()) {
        return false;
    }

    auto pair = m_allowed_values.insert(replacement);

    replacementNotice(iter, pair.first);

    m_allowed_values.erase(iter);
    return true;
}

template<class value_type, class Compare>
bool VariableDomain<value_type, Compare>::replaceAllowedValue(
    const value_type& to_replace,
    value_type&& replacement
) {
    auto iter = m_allowed_values.find(to_replace);
    if(iter == m_allowed_values.end()) {
        return false;
    }

    auto pair = m_allowed_values.insert(std::forward(replacement));

    replacementNotice(iter, pair.first);

    m_allowed_values.erase(iter);
    return true;
}

template<class value_type, class Compare>
void VariableDomain<value_type, Compare>::subscribeVariable(
    const DomainRestrictedVariable<value_type, Compare>* const ptr
) {
    m_managed_variables.insert(ptr);
}

template<class value_type, class Compare>
void VariableDomain<value_type, Compare>::unsubscribeVariable(
    const DomainRestrictedVariable<value_type, Compare>* const ptr
) {
    m_managed_variables.erase(ptr);
}

template<class value_type, class Compare>
void VariableDomain<value_type, Compare>::deletionNotice(
    value_type* to_delete
) {
    for(auto& var : m_managed_variables) {
        var->deletionNotice(to_delete);
    }
}

template<class value_type, class Compare>
void VariableDomain<value_type, Compare>::replacementNotice(
    value_type* to_replace,
    value_type* replacement
) {
    for(auto& var : m_managed_variables) {
        var->replacementNotice(to_replace, replacement);
    }
}

template<class value_type, class Compare>
DomainRestrictedVariable<value_type, Compare>::DomainRestrictedVariable(
    VariableDomain<value_type, Compare>& domain,
    const value_type& value
): m_domain(domain), m_value(domain.find(value))
{
    m_domain.get().subscribeVariable(this);
}

template<class value_type, class Compare>
DomainRestrictedVariable<value_type, Compare>::DomainRestrictedVariable(
    VariableDomain<value_type, Compare>& domain
): m_domain(domain), m_value(domain.m_allowed_values.end())
{
    m_domain.get().subscribeVariable(this);
}

template<class value_type, class Compare>
DomainRestrictedVariable<value_type, Compare>::DomainRestrictedVariable(
    const DomainRestrictedVariable& other
): m_domain(other.m_domain), m_value(other.m_value)
{
    m_domain.get().subscribeVariable(this);
}

template<class value_type, class Compare>
DomainRestrictedVariable<value_type, Compare>::DomainRestrictedVariable(
    DomainRestrictedVariable&& other
): m_domain(other.m_domain), m_value(other.m_value)
{
    other.clear();
    m_domain.get().unsubscribeVariable(&other);
    m_domain.get().subscribeVariable(this);
}

template<class value_type, class Compare>
DomainRestrictedVariable<value_type, Compare>::~DomainRestrictedVariable() {
    m_domain.get().unsubscribeVariable(this);
}

template<class value_type, class Compare>
DomainRestrictedVariable<value_type, Compare>&
    DomainRestrictedVariable<value_type, Compare>::operator=(
    const DomainRestrictedVariable& other
) {
    m_domain.get().unsubscribeVariable(this);
    m_domain = other.m_domain;
    m_domain.get().subscribeVariable(this);
    m_value = other.m_value;
}

template<class value_type, class Compare>
DomainRestrictedVariable<value_type, Compare>&
    DomainRestrictedVariable<value_type, Compare>::operator=(
    DomainRestrictedVariable&& other
) {
    m_domain.get().unsubscribeVariable(this);
    other.m_domain.get().unsubscribeVariable(&other);
    m_domain = other.m_domain;
    m_domain.get().subscribeVariable(this);
    m_value = other.m_value;
    other.clear();
}

template<class value_type, class Compare>
DomainRestrictedVariable<value_type, Compare>&
    DomainRestrictedVariable<value_type, Compare>::operator=(
    const value_type& value
) {
    m_value = m_domain.get().m_allowed_values.find(value);
}

template<class value_type, class Compare>
void DomainRestrictedVariable<value_type, Compare>::clear() {
    m_value = nullptr;
}

template<class value_type, class Compare>
bool DomainRestrictedVariable<value_type, Compare>::has_value() const {
    return m_value != nullptr;
}

template<class value_type, class Compare>
DomainRestrictedVariable<value_type, Compare>::operator value_type() {
    return *m_value;
}

template<class value_type, class Compare>
bool operator==(
    const DomainRestrictedVariable<value_type, Compare>& lhs,
    const DomainRestrictedVariable<value_type, Compare>& rhs
) {
    return Compare()(lhs, rhs) == Compare()(rhs, lhs);
}

template<class value_type, class Compare>
bool operator!=(
    const DomainRestrictedVariable<value_type, Compare>& lhs,
    const DomainRestrictedVariable<value_type, Compare>& rhs
) {
    return !(lhs == rhs);
}

template<class value_type, class Compare>
bool operator<(
    const DomainRestrictedVariable<value_type, Compare>& lhs,
    const DomainRestrictedVariable<value_type, Compare>& rhs
) {
    return Compare()(lhs, rhs);
}

template<class value_type, class Compare>
bool operator>(
    const DomainRestrictedVariable<value_type, Compare>& lhs,
    const DomainRestrictedVariable<value_type, Compare>& rhs
) {
    return rhs < lhs;
}

template<class value_type, class Compare>
bool operator<=(
    const DomainRestrictedVariable<value_type, Compare>& lhs,
    const DomainRestrictedVariable<value_type, Compare>& rhs
) {
    return !(lhs > rhs);
}

template<class value_type, class Compare>
bool operator>=(
    const DomainRestrictedVariable<value_type, Compare>& lhs,
    const DomainRestrictedVariable<value_type, Compare>& rhs
) {
    return !(lhs < rhs);
}

template<class value_type, class Compare>
void DomainRestrictedVariable<value_type, Compare>::deletionNotice(
    value_type* to_delete
) {
    if(m_value == to_delete) {
        clear();
    }
}

template<class value_type, class Compare>
void DomainRestrictedVariable<value_type, Compare>::replacementNotice(
    value_type* to_replace,
    value_type* replacement
) {
    if(m_value == to_replace) {
        m_value = replacement;
    }
}

#endif
