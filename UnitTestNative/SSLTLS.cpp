#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestNative
{
	TEST_CLASS(SSLTLS)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			HCERTSTORE hMyCertStore = NULL;
			PCCERT_CONTEXT aCertContext = NULL;

			hMyCertStore = CertOpenStore(CERT_STORE_PROV_SYSTEM,
				X509_ASN_ENCODING,
				0,
				CERT_SYSTEM_STORE_LOCAL_MACHINE,
				L"MY");

			if (hMyCertStore == NULL)
			{
				int a = 1;
			}
		}

	};
}