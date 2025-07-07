#include "contact.h"
// Dummy contact data
static Contact dummyContacts[] = {

    {"Alice Smith", "0987654321", "alice@example.com"},
    {"Bob Johnson", "1112223333", "bob@company.com"},
    {"Carol White", "4445556666", "carol@company.com"},
    {"David Brown", "7778889999", "david@example.com"},
    {"Eve Davis", "6665554444", "eve@example.com"}};

void populateAddressBook(AddressBook *addressBook)
{
    // printf("%zu\n", sizeof(dummyContacts));
    int numDummyContacts = sizeof(dummyContacts) / sizeof(dummyContacts[0]);
    for (int i = 0; i < numDummyContacts; i++)
    {
        addressBook->contacts[addressBook->contactCount++] = dummyContacts[i];
    }
}