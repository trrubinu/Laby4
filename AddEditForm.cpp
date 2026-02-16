#include "AddEditForm.h"
#include "DiscountStrategy.h"

namespace CargoCompanyWF {

    static double ParseDouble(String^ text)
    {
        if (String::IsNullOrWhiteSpace(text))
            throw gcnew FormatException("Пустая строка");
        String^ normalized = text->Trim()->Replace(",", ".");
        return Convert::ToDouble(normalized, System::Globalization::CultureInfo::InvariantCulture);
    }

    System::String^ FormUtf8(const char* utf8)
    {
        if (!utf8) return gcnew System::String(L"");
        int len = (int)strlen(utf8);
        array<unsigned char>^ bytes = gcnew array<unsigned char>(len);
        pin_ptr<const char> p = utf8;
        for (int i = 0; i < len; i++) bytes[i] = p[i];
        return System::Text::Encoding::UTF8->GetString(bytes);
    }

    AddEditForm::AddEditForm(CargoCompany* company, int mode)
        : company(company), mode(mode), editIndex(-1)
    {
        InitializeComponent();
    }

    AddEditForm::AddEditForm(CargoCompany* company, int mode, int index)
        : company(company), mode(mode), editIndex(index)
    {
        InitializeComponent();
        LoadDataForEdit();
    }

    void AddEditForm::InitializeComponent()
    {
        this->Text = (mode == 0) ? "Тариф" : (mode == 1) ? "Клиент" : "Заказ";
        this->Width = 350;
        this->Height = 300;
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
        this->StartPosition = FormStartPosition::CenterParent;

        lbl1 = gcnew Label();
        lbl1->Location = Point(20, 20);
        lbl1->AutoSize = true;
        this->Controls->Add(lbl1);

        txt1 = gcnew TextBox();
        txt1->Location = Point(120, 20);
        txt1->Width = 200;
        txt1->MaxLength = 100;
        this->Controls->Add(txt1);

        lbl2 = gcnew Label();
        lbl2->Location = Point(20, 60);
        lbl2->AutoSize = true;
        this->Controls->Add(lbl2);

        txt2 = gcnew TextBox();
        txt2->Location = Point(120, 60);
        txt2->Width = 200;
        this->Controls->Add(txt2);

        comboTariffs = gcnew ComboBox();
        comboTariffs->Location = Point(120, 60);
        comboTariffs->Width = 200;
        comboTariffs->DropDownStyle = ComboBoxStyle::DropDownList;
        this->Controls->Add(comboTariffs);

        lbl3 = gcnew Label();
        lbl3->Location = Point(20, 100);
        lbl3->AutoSize = true;
        this->Controls->Add(lbl3);

        txt3 = gcnew TextBox();
        txt3->Location = Point(120, 100);
        txt3->Width = 200;
        this->Controls->Add(txt3);

        comboDiscountType = gcnew ComboBox();
        comboDiscountType->Location = Point(120, 100);
        comboDiscountType->Width = 120;
        comboDiscountType->DropDownStyle = ComboBoxStyle::DropDownList;
        comboDiscountType->Items->Add("Без скидки");
        comboDiscountType->Items->Add("Процентная скидка");
        comboDiscountType->Items->Add("Фиксированная скидка");
        this->Controls->Add(comboDiscountType);

        txtDiscountValue = gcnew TextBox();
        txtDiscountValue->Location = Point(250, 100);
        txtDiscountValue->Width = 70;
        txtDiscountValue->Visible = false;
        this->Controls->Add(txtDiscountValue);

        btnOK = gcnew Button();
        btnOK->Text = "OK";
        btnOK->Location = Point(70, 220);
        btnOK->Click += gcnew EventHandler(this, &AddEditForm::OnOK);
        this->Controls->Add(btnOK);

        btnCancel = gcnew Button();
        btnCancel->Text = "Отмена";
        btnCancel->Location = Point(170, 220);
        btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
        this->Controls->Add(btnCancel);

        comboDiscountType->SelectedIndexChanged += gcnew EventHandler(this, &AddEditForm::OnDiscountTypeChanged);

        ConfigureMode();
    }

    void AddEditForm::ConfigureMode()
    {
        if (mode == 0)
        {
            this->Text = editIndex == -1 ? "Добавить тариф" : "Редактировать тариф";
            lbl1->Text = "Название:";
            lbl2->Text = "Цена:";
            lbl3->Text = "Скидка:";
            txt3->Visible = false;
            comboDiscountType->Visible = true;
            txtDiscountValue->Visible = comboDiscountType->SelectedIndex > 0;
            comboTariffs->Visible = false;
            txt2->Visible = true;
        }
        else if (mode == 1)
        {
            this->Text = editIndex == -1 ? "Добавить клиента" : "Редактировать клиента";
            lbl1->Text = "ФИО:";
            lbl2->Text = "Паспорт:";
            lbl3->Visible = false;
            txt3->Visible = false;
            comboDiscountType->Visible = false;
            txtDiscountValue->Visible = false;
            comboTariffs->Visible = false;
            txt2->Visible = true;
            txt2->MaxLength = 20;
        }
        else
        {
            this->Text = editIndex == -1 ? "Добавить заказ" : "Редактировать заказ";
            lbl1->Text = "Паспорт:";
            lbl2->Text = "Тариф:";
            lbl3->Text = "Объём:";
            comboDiscountType->Visible = false;
            txtDiscountValue->Visible = false;
            txt2->Visible = false;
            comboTariffs->Visible = true;

            comboTariffs->Items->Clear();
            for (const auto& tariff : company->getTariffs())
            {
                comboTariffs->Items->Add(gcnew String(tariff.getName().c_str()));
            }
        }
    }

    void AddEditForm::LoadDataForEdit()
    {
        if (mode == 0)
        {
            const auto& tariff = company->getTariffs()[editIndex];
            txt1->Text = gcnew String(tariff.getName().c_str());
            txt2->Text = tariff.getBasePrice().ToString();

            std::wstring discountInfo = tariff.getDiscountInfo();
            if (discountInfo.find(L"%") != std::wstring::npos)
            {
                comboDiscountType->SelectedIndex = 1;
                txtDiscountValue->Visible = true;
                size_t percentPos = discountInfo.find(L"%");
                std::wstring percentStr = discountInfo.substr(7, percentPos - 7);
                txtDiscountValue->Text = gcnew String(percentStr.c_str());
            }
            else if (discountInfo.find(L"Фиксированная скидка") != std::wstring::npos)
            {
                comboDiscountType->SelectedIndex = 2;
                txtDiscountValue->Visible = true;
                std::wstring discountStr = discountInfo.substr(17);
                txtDiscountValue->Text = gcnew String(discountStr.c_str());
            }
            else
            {
                comboDiscountType->SelectedIndex = 0;
                txtDiscountValue->Visible = false;
            }
        }
        else if (mode == 1)
        {
            const auto& client = company->getClients()[editIndex];
            txt1->Text = gcnew String(client.getName().c_str());
            txt2->Text = gcnew String(client.getPassport().c_str());
        }
        else
        {
            const auto& order = company->getOrders()[editIndex];
            txt1->Text = gcnew String(order.getClientPassport().c_str());
            txt3->Text = order.getVolume().ToString();

            for (int i = 0; i < comboTariffs->Items->Count; ++i)
            {
                if (comboTariffs->Items[i]->ToString()->Equals(gcnew String(order.getTariffName().c_str())))
                {
                    comboTariffs->SelectedIndex = i;
                    break;
                }
            }
        }
    }

    void AddEditForm::OnDiscountTypeChanged(Object^ sender, EventArgs^ e)
    {
        txtDiscountValue->Visible = comboDiscountType->SelectedIndex > 0;
    }

    bool AddEditForm::ValidateInputs()
    {
        if (mode == 0)
        {
            if (String::IsNullOrWhiteSpace(txt1->Text) || txt1->Text->Length > 100)
            {
                MessageBox::Show(L"Название тарифа должно быть от 1 до 100 символов");
                return false;
            }

            try
            {
                double price = ParseDouble(txt2->Text);
                if (price <= 0 || price > 1000000)
                {
                    MessageBox::Show(L"Цена должна быть положительным числом и не превышать 1,000,000");
                    return false;
                }
            }
            catch (FormatException^)
            {
                MessageBox::Show(L"Некорректный формат цены");
                return false;
            }

            if (comboDiscountType->SelectedIndex > 0 && txtDiscountValue->Visible)
            {
                try
                {
                    double discountValue = ParseDouble(txtDiscountValue->Text);
                    if (discountValue <= 0)
                    {
                        MessageBox::Show(L"Значение скидки должно быть положительным числом");
                        return false;
                    }

                    if (comboDiscountType->SelectedIndex == 1 && discountValue > 100)
                    {
                        MessageBox::Show(L"Процент скидки не может превышать 100%");
                        return false;
                    }
                }
                catch (FormatException^)
                {
                    MessageBox::Show(L"Некорректный формат значения скидки");
                    return false;
                }
            }
        }
        else if (mode == 1)
        {
            if (String::IsNullOrWhiteSpace(txt1->Text) || txt1->Text->Length > 100)
            {
                MessageBox::Show(L"ФИО должно быть от 1 до 100 символов");
                return false;
            }

            if (String::IsNullOrWhiteSpace(txt2->Text) || txt2->Text->Length > 20)
            {
                MessageBox::Show(L"Паспорт должен быть от 1 до 20 символов");
                return false;
            }
        }
        else
        {
            if (comboTariffs->Items->Count == 0)
            {
                MessageBox::Show(L"Добавьте хотя бы один тариф перед созданием заказа.");
                return false;
            }

            if (String::IsNullOrWhiteSpace(txt1->Text) || txt1->Text->Length > 20)
            {
                MessageBox::Show(L"Паспорт должен быть от 1 до 20 символов");
                return false;
            }

            if (comboTariffs->SelectedIndex == -1)
            {
                MessageBox::Show(L"Выберите тариф");
                return false;
            }

            try
            {
                double volume = ParseDouble(txt3->Text);
                if (volume <= 0 || volume > 1000000)
                {
                    MessageBox::Show(L"Объём должен быть положительным числом и не превышать 1,000,000");
                    return false;
                }
            }
            catch (FormatException^)
            {
                MessageBox::Show(L"Некорректный формат объёма");
                return false;
            }
        }

        return true;
    }

    void AddEditForm::OnOK(Object^ sender, EventArgs^ e)
    {
        if (!ValidateInputs())
            return;

        try
        {
            if (mode == 0)
            {
                std::wstring name = msclr::interop::marshal_as<std::wstring>(txt1->Text);
                double price = ParseDouble(txt2->Text);
                IDiscountStrategy* strategy = nullptr;

                if (comboDiscountType->SelectedIndex == 1)
                {
                    double percent = ParseDouble(txtDiscountValue->Text);
                    strategy = new PercentageDiscountStrategy(percent);
                }
                else if (comboDiscountType->SelectedIndex == 2)
                {
                    double discount = ParseDouble(txtDiscountValue->Text);
                    strategy = new FixedDiscountStrategy(discount);
                }
                else
                {
                    strategy = new NoDiscountStrategy();
                }

                if (editIndex == -1)
                {
                    company->addTariff(name, price, strategy);
                }
                else
                {
                    company->updateTariff(editIndex, name, price, strategy);
                }
            }
            else if (mode == 1)
            {
                std::wstring name = msclr::interop::marshal_as<std::wstring>(txt1->Text);
                std::wstring passport = msclr::interop::marshal_as<std::wstring>(txt2->Text);

                if (editIndex == -1)
                {
                    company->addClient(name, passport);
                }
                else
                {
                    company->updateClient(editIndex, name, passport);
                }
            }
            else
            {
                std::wstring passport = msclr::interop::marshal_as<std::wstring>(txt1->Text);
                std::wstring tariff = msclr::interop::marshal_as<std::wstring>(comboTariffs->SelectedItem->ToString());
                double volume = ParseDouble(txt3->Text);

                if (editIndex == -1)
                {
                    company->createOrder(passport, tariff, volume);
                }
                else
                {
                    company->updateOrder(editIndex, passport, tariff, volume);
                }
            }

            this->DialogResult = System::Windows::Forms::DialogResult::OK;
            this->Close();
        }
        catch (std::exception& ex)
        {
            MessageBox::Show(FormUtf8(ex.what()), L"Ошибка");
        }
    }
}
