#include "StatisticsForm.h"

namespace CargoCompanyWF {

    StatisticsForm::StatisticsForm(CargoCompany* company)
        : company(company)
    {
        InitializeComponent();
        RefreshStats();
    }

    void StatisticsForm::InitializeComponent()
    {
        this->Text = L"Статистика";
        this->Width = 420;
        this->Height = 420;
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
        this->StartPosition = FormStartPosition::CenterParent;
        this->MaximizeBox = false;

        lblStats = gcnew Label();
        lblStats->Location = Point(20, 20);
        lblStats->Size = Drawing::Size(360, 100);
        lblStats->AutoSize = false;
        lblStats->Font = gcnew Drawing::Font("Segoe UI", 10);
        this->Controls->Add(lblStats);

        lblMinTariff = gcnew Label();
        lblMinTariff->Location = Point(20, 125);
        lblMinTariff->Size = Drawing::Size(360, 45);
        lblMinTariff->AutoSize = false;
        lblMinTariff->Font = gcnew Drawing::Font("Segoe UI", 9);
        this->Controls->Add(lblMinTariff);

        lblClientSumCaption = gcnew Label();
        lblClientSumCaption->Text = L"Сумма заказов клиента:";
        lblClientSumCaption->Location = Point(20, 180);
        lblClientSumCaption->AutoSize = true;
        this->Controls->Add(lblClientSumCaption);

        txtPassport = gcnew TextBox();
        txtPassport->Location = Point(20, 205);
        txtPassport->Width = 180;
        txtPassport->MaxLength = 20;
        this->Controls->Add(txtPassport);

        btnShowClientSum = gcnew Button();
        btnShowClientSum->Text = L"Показать";
        btnShowClientSum->Location = Point(210, 203);
        btnShowClientSum->Click += gcnew EventHandler(this, &StatisticsForm::OnShowClientSumClick);
        this->Controls->Add(btnShowClientSum);

        lblClientSumResult = gcnew Label();
        lblClientSumResult->Location = Point(20, 235);
        lblClientSumResult->Size = Drawing::Size(360, 25);
        lblClientSumResult->Font = gcnew Drawing::Font("Segoe UI", 9, FontStyle::Bold);
        this->Controls->Add(lblClientSumResult);

        lblTotalCaption = gcnew Label();
        lblTotalCaption->Text = L"Суммарная стоимость всех заказов:";
        lblTotalCaption->Location = Point(20, 270);
        lblTotalCaption->AutoSize = true;
        this->Controls->Add(lblTotalCaption);

        lblTotalResult = gcnew Label();
        lblTotalResult->Location = Point(20, 295);
        lblTotalResult->Size = Drawing::Size(360, 22);
        lblTotalResult->Font = gcnew Drawing::Font("Segoe UI", 10, FontStyle::Bold);
        this->Controls->Add(lblTotalResult);

        btnRefresh = gcnew Button();
        btnRefresh->Text = L"Обновить";
        btnRefresh->Location = Point(20, 328);
        btnRefresh->Click += gcnew EventHandler(this, &StatisticsForm::OnRefreshClick);
        this->Controls->Add(btnRefresh);

        btnClose = gcnew Button();
        btnClose->Text = L"Закрыть";
        btnClose->Location = Point(120, 328);
        btnClose->DialogResult = System::Windows::Forms::DialogResult::OK;
        this->Controls->Add(btnClose);
    }

    void StatisticsForm::RefreshStats()
    {
        size_t tc = company->getTariffCount();
        size_t cc = company->getClientCount();
        size_t oc = company->getOrderCount();
        String^ stats = String::Format(L"Количество тарифов: {0}\nКоличество клиентов: {1}\nКоличество заказов: {2}",
            (int)tc, (int)cc, (int)oc);
        lblStats->Text = stats;

        const Tariff* minT = company->findMinPriceTariff();
        if (minT)
        {
            String^ minInfo = String::Format(L"Тариф с мин. ценой: {0} ({1:F2} RUB/тонна)",
                gcnew String(minT->getName().c_str()), minT->getFinalPrice());
            lblMinTariff->Text = minInfo;
        }
        else
            lblMinTariff->Text = L"Тарифы отсутствуют.";

        double total = company->getTotalAll();
        lblTotalResult->Text = String::Format(L"{0:F2} RUB", total);

        lblClientSumResult->Text = L"";
    }

    void StatisticsForm::OnShowClientSumClick(Object^ sender, EventArgs^ e)
    {
        if (String::IsNullOrWhiteSpace(txtPassport->Text))
        {
            lblClientSumResult->Text = L"Введите номер паспорта.";
            return;
        }
        std::wstring passport = msclr::interop::marshal_as<std::wstring>(txtPassport->Text->Trim());
        if (!company->clientExists(passport))
        {
            lblClientSumResult->Text = L"Клиент не найден.";
            return;
        }
        double sum = company->getTotalForClient(passport);
        lblClientSumResult->Text = String::Format(L"{0:F2} RUB", sum);
    }

    void StatisticsForm::OnRefreshClick(Object^ sender, EventArgs^ e)
    {
        RefreshStats();
    }
}
