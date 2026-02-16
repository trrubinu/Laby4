#pragma once

#include "CargoCompany.h"
#include <msclr/marshal_cppstd.h>

namespace CargoCompanyWF {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class StatisticsForm : public Form
    {
    public:
        StatisticsForm(CargoCompany* company);

    private:
        CargoCompany* company;
        Label^ lblStats;
        Label^ lblMinTariff;
        Label^ lblClientSumCaption;
        TextBox^ txtPassport;
        Button^ btnShowClientSum;
        Label^ lblClientSumResult;
        Label^ lblTotalCaption;
        Label^ lblTotalResult;
        Button^ btnRefresh;
        Button^ btnClose;

        void InitializeComponent();
        void RefreshStats();

        void OnShowClientSumClick(Object^ sender, EventArgs^ e);
        void OnRefreshClick(Object^ sender, EventArgs^ e);
    };
}
