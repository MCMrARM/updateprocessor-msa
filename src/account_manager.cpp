#include <msa/account_manager.h>
#include <msa/account.h>
#include <msa/storage_manager.h>

using namespace msa;

void AccountManager::addAccount(std::shared_ptr<Account> account) {
    if (!accounts.insert({account->getCID(), account}).second)
        throw std::runtime_error("Account with this CID ia already added");
}

void AccountManager::removeAccount(Account& account) {
    auto it = accounts.find(account.getCID());
    if (it == accounts.end())
        throw std::runtime_error("No such account found to delete");
    if (it->second.get() != &account)
        throw std::runtime_error("An account with a CID of the same value as the specified account does exist, "
                                         "but is not the specified instance");
    accounts.erase(it);
}

std::shared_ptr<Account> AccountManager::findAccount(std::string const& cid) {
    auto it = accounts.find(cid);
    if (it != accounts.end())
        return it->second;
    try {
        auto account = storageManager.readAccount(cid);
        if (account) {
            addAccount(account);
            return account;
        }
    } catch (std::exception& e) {
    }
    return std::shared_ptr<Account>();
}