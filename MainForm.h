#pragma once

#include "CargoCompany.h"
#include "AddEditForm.h"
#include "StatisticsForm.h"
#include <msclr/marshal_cppstd.h>
#include <vcclr.h>

namespace CargoCompanyWF {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    System::String^ FromUtf8(const char* utf8);

    public ref class MainForm : public Form
    {
    public:
        MainForm(void);

    protected:
        ~MainForm();

    private:
        System::ComponentModel::Container ^components;
        CargoCompany* company;

        DataGridView^ dataGridTariffs;
        DataGridView^ dataGridClients;
        DataGridView^ dataGridOrders;

        Button^ btnAddTariff;
        Button^ btnDeleteTariff;
        Button^ btnEditTariff;

        Button^ btnAddClient;
        Button^ btnDeleteClient;
        Button^ btnEditClient;

        Button^ btnAddOrder;
        Button^ btnDeleteOrder;
        Button^ btnEditOrder;

        Button^ btnOpenFile;
        Button^ btnSaveFile;

        ComboBox^ comboSortTariffs;
        Button^ btnSortTariffs;
        ComboBox^ comboSortClients;
        Button^ btnSortClients;
        ComboBox^ comboSortOrders;
        Button^ btnSortOrders;

        void InitializeComponent(void);
        void RefreshTables();
        void RefreshTariffsTable();
        void RefreshClientsTable();
        void RefreshOrdersTable();

        void OnAddTariffClick(Object^ sender, EventArgs^ e);
        void OnEditTariffClick(Object^ sender, EventArgs^ e);
        void OnDeleteTariffClick(Object^ sender, EventArgs^ e);
        void OnAddClientClick(Object^ sender, EventArgs^ e);
        void OnEditClientClick(Object^ sender, EventArgs^ e);
        void OnDeleteClientClick(Object^ sender, EventArgs^ e);
        void OnAddOrderClick(Object^ sender, EventArgs^ e);
        void OnEditOrderClick(Object^ sender, EventArgs^ e);
        void OnDeleteOrderClick(Object^ sender, EventArgs^ e);
        void OnSortTariffsClick(Object^ sender, EventArgs^ e);
        void OnSortClientsClick(Object^ sender, EventArgs^ e);
        void OnSortOrdersClick(Object^ sender, EventArgs^ e);
        void OnOpenFileClick(Object^ sender, EventArgs^ e);
        void OnSaveFileClick(Object^ sender, EventArgs^ e);
        void OnStatisticsClick(Object^ sender, EventArgs^ e);
    };
}
