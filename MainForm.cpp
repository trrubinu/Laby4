#include "MainForm.h"

namespace CargoCompanyWF {

    System::String^ FromUtf8(const char* utf8)
    {
        if (!utf8) return gcnew System::String(L"");
        int len = (int)strlen(utf8);
        array<unsigned char>^ bytes = gcnew array<unsigned char>(len);
        pin_ptr<const char> p = utf8;
        for (int i = 0; i < len; i++) bytes[i] = p[i];
        return System::Text::Encoding::UTF8->GetString(bytes);
    }

    MainForm::MainForm(void)
    {
        InitializeComponent();
        company = new CargoCompany();
        RefreshTables();
    }

    MainForm::~MainForm()
    {
        if (components)
        {
            delete components;
        }
        if (company != nullptr) {
            delete company;
            company = nullptr;
        }
    }

    void MainForm::InitializeComponent(void)
    {
        this->components = gcnew System::ComponentModel::Container();
        this->Text = L"Грузовая Компания";
        this->Width = 1000;
        this->Height = 800;
        this->StartPosition = FormStartPosition::CenterScreen;
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
        this->MaximizeBox = false;

        Label^ lblTariffs = gcnew Label();
        lblTariffs->Text = L"Тарифы";
        lblTariffs->Font = gcnew Drawing::Font("Segoe UI", 12, FontStyle::Bold);
        lblTariffs->Location = Point(20, 10);
        lblTariffs->AutoSize = true;
        this->Controls->Add(lblTariffs);

        dataGridTariffs = gcnew DataGridView();
        dataGridTariffs->Location = Point(20, 40);
        dataGridTariffs->Size = Drawing::Size(940, 150);
        dataGridTariffs->AllowUserToAddRows = false;
        dataGridTariffs->AllowUserToDeleteRows = false;
        dataGridTariffs->ColumnCount = 4;
        dataGridTariffs->Columns[0]->Name = L"Название";
        dataGridTariffs->Columns[1]->Name = L"Базовая цена";
        dataGridTariffs->Columns[2]->Name = L"Итоговая цена";
        dataGridTariffs->Columns[3]->Name = L"Скидка";
        dataGridTariffs->Columns[0]->ReadOnly = true;
        dataGridTariffs->Columns[1]->ReadOnly = true;
        dataGridTariffs->Columns[2]->ReadOnly = true;
        dataGridTariffs->Columns[3]->ReadOnly = true;
        this->Controls->Add(dataGridTariffs);

        btnAddTariff = gcnew Button();
        btnAddTariff->Text = L"Добавить тариф";
        btnAddTariff->Location = Point(20, 200);
        btnAddTariff->Click += gcnew EventHandler(this, &MainForm::OnAddTariffClick);
        this->Controls->Add(btnAddTariff);

        btnEditTariff = gcnew Button();
        btnEditTariff->Text = L"Редактировать тариф";
        btnEditTariff->Location = Point(150, 200);
        btnEditTariff->Click += gcnew EventHandler(this, &MainForm::OnEditTariffClick);
        this->Controls->Add(btnEditTariff);

        btnDeleteTariff = gcnew Button();
        btnDeleteTariff->Text = L"Удалить тариф";
        btnDeleteTariff->Location = Point(280, 200);
        btnDeleteTariff->Click += gcnew EventHandler(this, &MainForm::OnDeleteTariffClick);
        this->Controls->Add(btnDeleteTariff);

        comboSortTariffs = gcnew ComboBox();
        comboSortTariffs->Location = Point(410, 200);
        comboSortTariffs->Width = 180;
        comboSortTariffs->DropDownStyle = ComboBoxStyle::DropDownList;
        comboSortTariffs->Items->Add(L"Название (возр.)");
        comboSortTariffs->Items->Add(L"Название (убыв.)");
        comboSortTariffs->Items->Add(L"Базовая цена (возр.)");
        comboSortTariffs->Items->Add(L"Базовая цена (убыв.)");
        comboSortTariffs->Items->Add(L"Итоговая цена (возр.)");
        comboSortTariffs->Items->Add(L"Итоговая цена (убыв.)");
        comboSortTariffs->Items->Add(L"Скидка (возр.)");
        comboSortTariffs->Items->Add(L"Скидка (убыв.)");
        comboSortTariffs->SelectedIndex = 0;
        this->Controls->Add(comboSortTariffs);

        btnSortTariffs = gcnew Button();
        btnSortTariffs->Text = L"Сортировать";
        btnSortTariffs->Location = Point(600, 198);
        btnSortTariffs->Click += gcnew EventHandler(this, &MainForm::OnSortTariffsClick);
        this->Controls->Add(btnSortTariffs);

        Label^ lblClients = gcnew Label();
        lblClients->Text = L"Клиенты";
        lblClients->Font = gcnew Drawing::Font("Segoe UI", 12, FontStyle::Bold);
        lblClients->Location = Point(20, 240);
        lblClients->AutoSize = true;
        this->Controls->Add(lblClients);

        dataGridClients = gcnew DataGridView();
        dataGridClients->Location = Point(20, 270);
        dataGridClients->Size = Drawing::Size(940, 150);
        dataGridClients->AllowUserToAddRows = false;
        dataGridClients->AllowUserToDeleteRows = false;
        dataGridClients->ColumnCount = 3;
        dataGridClients->Columns[0]->Name = L"ФИО";
        dataGridClients->Columns[1]->Name = L"Паспорт";
        dataGridClients->Columns[2]->Name = L"Индекс";
        dataGridClients->Columns[2]->Visible = false;
        dataGridClients->Columns[0]->ReadOnly = true;
        dataGridClients->Columns[1]->ReadOnly = true;
        this->Controls->Add(dataGridClients);

        btnAddClient = gcnew Button();
        btnAddClient->Text = L"Добавить клиента";
        btnAddClient->Location = Point(20, 430);
        btnAddClient->Click += gcnew EventHandler(this, &MainForm::OnAddClientClick);
        this->Controls->Add(btnAddClient);

        btnEditClient = gcnew Button();
        btnEditClient->Text = L"Редактировать клиента";
        btnEditClient->Location = Point(150, 430);
        btnEditClient->Click += gcnew EventHandler(this, &MainForm::OnEditClientClick);
        this->Controls->Add(btnEditClient);

        btnDeleteClient = gcnew Button();
        btnDeleteClient->Text = L"Удалить клиента";
        btnDeleteClient->Location = Point(280, 430);
        btnDeleteClient->Click += gcnew EventHandler(this, &MainForm::OnDeleteClientClick);
        this->Controls->Add(btnDeleteClient);

        comboSortClients = gcnew ComboBox();
        comboSortClients->Location = Point(410, 428);
        comboSortClients->Width = 180;
        comboSortClients->DropDownStyle = ComboBoxStyle::DropDownList;
        comboSortClients->Items->Add(L"ФИО (возр.)");
        comboSortClients->Items->Add(L"ФИО (убыв.)");
        comboSortClients->Items->Add(L"Паспорт (возр.)");
        comboSortClients->Items->Add(L"Паспорт (убыв.)");
        comboSortClients->SelectedIndex = 0;
        this->Controls->Add(comboSortClients);

        btnSortClients = gcnew Button();
        btnSortClients->Text = L"Сортировать";
        btnSortClients->Location = Point(600, 426);
        btnSortClients->Click += gcnew EventHandler(this, &MainForm::OnSortClientsClick);
        this->Controls->Add(btnSortClients);

        Label^ lblOrders = gcnew Label();
        lblOrders->Text = L"Заказы";
        lblOrders->Font = gcnew Drawing::Font("Segoe UI", 12, FontStyle::Bold);
        lblOrders->Location = Point(20, 470);
        lblOrders->AutoSize = true;
        this->Controls->Add(lblOrders);

        dataGridOrders = gcnew DataGridView();
        dataGridOrders->Location = Point(20, 500);
        dataGridOrders->Size = Drawing::Size(940, 150);
        dataGridOrders->AllowUserToAddRows = false;
        dataGridOrders->AllowUserToDeleteRows = false;
        dataGridOrders->ColumnCount = 5;
        dataGridOrders->Columns[0]->Name = L"Паспорт";
        dataGridOrders->Columns[1]->Name = L"Тариф";
        dataGridOrders->Columns[2]->Name = L"Объём";
        dataGridOrders->Columns[3]->Name = L"Стоимость";
        dataGridOrders->Columns[4]->Name = L"Индекс";
        dataGridOrders->Columns[4]->Visible = false;
        dataGridOrders->Columns[0]->ReadOnly = true;
        dataGridOrders->Columns[1]->ReadOnly = true;
        dataGridOrders->Columns[2]->ReadOnly = true;
        dataGridOrders->Columns[3]->ReadOnly = true;
        this->Controls->Add(dataGridOrders);

        btnAddOrder = gcnew Button();
        btnAddOrder->Text = L"Добавить заказ";
        btnAddOrder->Location = Point(20, 660);
        btnAddOrder->Click += gcnew EventHandler(this, &MainForm::OnAddOrderClick);
        this->Controls->Add(btnAddOrder);

        btnEditOrder = gcnew Button();
        btnEditOrder->Text = L"Редактировать заказ";
        btnEditOrder->Location = Point(150, 660);
        btnEditOrder->Click += gcnew EventHandler(this, &MainForm::OnEditOrderClick);
        this->Controls->Add(btnEditOrder);

        btnDeleteOrder = gcnew Button();
        btnDeleteOrder->Text = L"Удалить заказ";
        btnDeleteOrder->Location = Point(280, 660);
        btnDeleteOrder->Click += gcnew EventHandler(this, &MainForm::OnDeleteOrderClick);
        this->Controls->Add(btnDeleteOrder);

        comboSortOrders = gcnew ComboBox();
        comboSortOrders->Location = Point(410, 658);
        comboSortOrders->Width = 180;
        comboSortOrders->DropDownStyle = ComboBoxStyle::DropDownList;
        comboSortOrders->Items->Add(L"Паспорт (возр.)");
        comboSortOrders->Items->Add(L"Паспорт (убыв.)");
        comboSortOrders->Items->Add(L"Тариф (возр.)");
        comboSortOrders->Items->Add(L"Тариф (убыв.)");
        comboSortOrders->Items->Add(L"Объём (возр.)");
        comboSortOrders->Items->Add(L"Объём (убыв.)");
        comboSortOrders->Items->Add(L"Стоимость (возр.)");
        comboSortOrders->Items->Add(L"Стоимость (убыв.)");
        comboSortOrders->SelectedIndex = 0;
        this->Controls->Add(comboSortOrders);

        btnSortOrders = gcnew Button();
        btnSortOrders->Text = L"Сортировать";
        btnSortOrders->Location = Point(600, 656);
        btnSortOrders->Click += gcnew EventHandler(this, &MainForm::OnSortOrdersClick);
        this->Controls->Add(btnSortOrders);

        btnOpenFile = gcnew Button();
        btnOpenFile->Text = L"Открыть файл";
        btnOpenFile->Location = Point(20, 710);
        btnOpenFile->Click += gcnew EventHandler(this, &MainForm::OnOpenFileClick);
        this->Controls->Add(btnOpenFile);

        btnSaveFile = gcnew Button();
        btnSaveFile->Text = L"Сохранить в файл";
        btnSaveFile->Location = Point(150, 710);
        btnSaveFile->Click += gcnew EventHandler(this, &MainForm::OnSaveFileClick);
        this->Controls->Add(btnSaveFile);

        Button^ btnStatistics = gcnew Button();
        btnStatistics->Text = L"Статистика";
        btnStatistics->Location = Point(280, 710);
        btnStatistics->Click += gcnew EventHandler(this, &MainForm::OnStatisticsClick);
        this->Controls->Add(btnStatistics);
    }

    void MainForm::RefreshTables()
    {
        RefreshTariffsTable();
        RefreshClientsTable();
        RefreshOrdersTable();
    }

    void MainForm::RefreshTariffsTable()
    {
        dataGridTariffs->Rows->Clear();
        const auto& tariffs = company->getTariffs();
        for (size_t i = 0; i < tariffs.size(); ++i)
        {
            const auto& tariff = tariffs[i];
            dataGridTariffs->Rows->Add(
                gcnew String(tariff.getName().c_str()),
                tariff.getBasePrice(),
                tariff.getFinalPrice(),
                gcnew String(tariff.getDiscountDisplayInfo().c_str())
            );
        }
    }

    void MainForm::RefreshClientsTable()
    {
        dataGridClients->Rows->Clear();
        const auto& clients = company->getClients();
        for (size_t i = 0; i < clients.size(); ++i)
        {
            const auto& client = clients[i];
            dataGridClients->Rows->Add(
                gcnew String(client.getName().c_str()),
                gcnew String(client.getPassport().c_str()),
                (int)i
            );
        }
    }

    void MainForm::RefreshOrdersTable()
    {
        dataGridOrders->Rows->Clear();
        const auto& orders = company->getOrders();
        for (size_t i = 0; i < orders.size(); ++i)
        {
            const auto& order = orders[i];
            dataGridOrders->Rows->Add(
                gcnew String(order.getClientPassport().c_str()),
                gcnew String(order.getTariffName().c_str()),
                order.getVolume(),
                order.getCost(),
                (int)i
            );
        }
    }

    void MainForm::OnAddTariffClick(Object^ sender, EventArgs^ e)
    {
        AddEditForm^ form = gcnew AddEditForm(company, 0);
        if (form->ShowDialog() == System::Windows::Forms::DialogResult::OK)
            RefreshTariffsTable();
    }

    void MainForm::OnEditTariffClick(Object^ sender, EventArgs^ e)
    {
        if (dataGridTariffs->SelectedRows->Count > 0)
        {
            int index = dataGridTariffs->SelectedRows[0]->Index;
            AddEditForm^ form = gcnew AddEditForm(company, 0, index);
            if (form->ShowDialog() == System::Windows::Forms::DialogResult::OK)
                RefreshTariffsTable();
        }
    }

    void MainForm::OnDeleteTariffClick(Object^ sender, EventArgs^ e)
    {
        if (dataGridTariffs->SelectedRows->Count == 0) return;
        if (MessageBox::Show(L"Вы уверены, что хотите удалить выбранный тариф?", L"Подтверждение",
            MessageBoxButtons::YesNo, MessageBoxIcon::Question) != System::Windows::Forms::DialogResult::Yes)
            return;

        int index = dataGridTariffs->SelectedRows[0]->Index;
        try
        {
            company->removeTariff(index);
            RefreshTariffsTable();
        }
        catch (std::exception& ex)
        {
            MessageBox::Show(FromUtf8(ex.what()), L"Ошибка");
        }
    }

    void MainForm::OnAddClientClick(Object^ sender, EventArgs^ e)
    {
        AddEditForm^ form = gcnew AddEditForm(company, 1);
        if (form->ShowDialog() == System::Windows::Forms::DialogResult::OK)
            RefreshClientsTable();
    }

    void MainForm::OnEditClientClick(Object^ sender, EventArgs^ e)
    {
        if (dataGridClients->SelectedRows->Count > 0)
        {
            int index = dataGridClients->SelectedRows[0]->Index;
            AddEditForm^ form = gcnew AddEditForm(company, 1, index);
            if (form->ShowDialog() == System::Windows::Forms::DialogResult::OK)
                RefreshClientsTable();
        }
    }

    void MainForm::OnDeleteClientClick(Object^ sender, EventArgs^ e)
    {
        if (dataGridClients->SelectedRows->Count == 0) return;
        if (MessageBox::Show(L"Вы уверены, что хотите удалить выбранного клиента?", L"Подтверждение",
            MessageBoxButtons::YesNo, MessageBoxIcon::Question) != System::Windows::Forms::DialogResult::Yes)
            return;

        int index = dataGridClients->SelectedRows[0]->Index;
        try
        {
            company->removeClient(index);
            RefreshClientsTable();
        }
        catch (std::exception& ex)
        {
            MessageBox::Show(FromUtf8(ex.what()), L"Ошибка");
        }
    }

    void MainForm::OnAddOrderClick(Object^ sender, EventArgs^ e)
    {
        if (company->getTariffCount() == 0)
        {
            MessageBox::Show(L"Добавьте хотя бы один тариф перед созданием заказа.", L"Ошибка");
            return;
        }
        if (company->getClientCount() == 0)
        {
            MessageBox::Show(L"Добавьте хотя бы одного клиента перед созданием заказа.", L"Ошибка");
            return;
        }
        AddEditForm^ form = gcnew AddEditForm(company, 2);
        if (form->ShowDialog() == System::Windows::Forms::DialogResult::OK)
            RefreshOrdersTable();
    }

    void MainForm::OnEditOrderClick(Object^ sender, EventArgs^ e)
    {
        if (dataGridOrders->SelectedRows->Count > 0)
        {
            int index = dataGridOrders->SelectedRows[0]->Index;
            AddEditForm^ form = gcnew AddEditForm(company, 2, index);
            if (form->ShowDialog() == System::Windows::Forms::DialogResult::OK)
                RefreshOrdersTable();
        }
    }

    void MainForm::OnDeleteOrderClick(Object^ sender, EventArgs^ e)
    {
        if (dataGridOrders->SelectedRows->Count == 0) return;
        if (MessageBox::Show(L"Вы уверены, что хотите удалить выбранный заказ?", L"Подтверждение",
            MessageBoxButtons::YesNo, MessageBoxIcon::Question) != System::Windows::Forms::DialogResult::Yes)
            return;

        int index = dataGridOrders->SelectedRows[0]->Index;
        try
        {
            company->removeOrder(index);
            RefreshOrdersTable();
        }
        catch (std::exception& ex)
        {
            MessageBox::Show(FromUtf8(ex.what()), L"Ошибка");
        }
    }

    void MainForm::OnSortTariffsClick(Object^ sender, EventArgs^ e)
    {
        int idx = comboSortTariffs->SelectedIndex;
        if (idx < 0) return;
        int col = idx / 2;
        bool asc = (idx % 2) == 0;
        company->sortTariffs(col, asc);
        RefreshTariffsTable();
    }

    void MainForm::OnSortClientsClick(Object^ sender, EventArgs^ e)
    {
        int idx = comboSortClients->SelectedIndex;
        if (idx < 0) return;
        int col = idx / 2;
        bool asc = (idx % 2) == 0;
        company->sortClients(col, asc);
        RefreshClientsTable();
    }

    void MainForm::OnSortOrdersClick(Object^ sender, EventArgs^ e)
    {
        int idx = comboSortOrders->SelectedIndex;
        if (idx < 0) return;
        int col = idx / 2;
        bool asc = (idx % 2) == 0;
        company->sortOrders(col, asc);
        RefreshOrdersTable();
    }

    void MainForm::OnOpenFileClick(Object^ sender, EventArgs^ e)
    {
        OpenFileDialog^ dlg = gcnew OpenFileDialog();
        dlg->Filter = L"Файлы данных (*.txt)|*.txt|Все файлы (*.*)|*.*";
        dlg->FilterIndex = 1;
        if (dlg->ShowDialog() == System::Windows::Forms::DialogResult::OK)
        {
            try
            {
                std::wstring path = msclr::interop::marshal_as<std::wstring>(dlg->FileName);
                company->loadFromFile(path);
                RefreshTables();
                MessageBox::Show(L"Данные загружены.", L"Файл");
            }
            catch (std::exception& ex)
            {
                MessageBox::Show(FromUtf8(ex.what()), L"Ошибка");
            }
        }
    }

    void MainForm::OnSaveFileClick(Object^ sender, EventArgs^ e)
    {
        SaveFileDialog^ dlg = gcnew SaveFileDialog();
        dlg->Filter = L"Файлы данных (*.txt)|*.txt|Все файлы (*.*)|*.*";
        dlg->FilterIndex = 1;
        if (dlg->ShowDialog() == System::Windows::Forms::DialogResult::OK)
        {
            try
            {
                std::wstring path = msclr::interop::marshal_as<std::wstring>(dlg->FileName);
                company->saveToFile(path);
                MessageBox::Show(L"Данные сохранены.", L"Файл");
            }
            catch (std::exception& ex)
            {
                MessageBox::Show(FromUtf8(ex.what()), L"Ошибка");
            }
        }
    }

    void MainForm::OnStatisticsClick(Object^ sender, EventArgs^ e)
    {
        StatisticsForm^ form = gcnew StatisticsForm(company);
        form->ShowDialog();
    }
}
