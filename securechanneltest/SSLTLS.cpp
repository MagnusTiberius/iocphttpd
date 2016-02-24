#include "stdafx.h"
#include "CppUnitTest.h"

#define MY_ENCODING_TYPE  (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestNative
{
	TEST_CLASS(SSLTLS)
	{
	public:

		// Before running the test, please see \www\cert\ directory for info on how 
		// to create a cert and install it to your test machine.
		TEST_METHOD(TestMethod1)
		{
			HCERTSTORE      hSystemStore;
			PCERT_INFO      pTargetCertInfo;
			PCCERT_CONTEXT  pTargetCert = NULL;
			char            szSubjectName[] = "IOCPHTTPRoot";

			hSystemStore = CertOpenStore(CERT_STORE_PROV_SYSTEM,
				0,
				NULL,
				CERT_SYSTEM_STORE_CURRENT_USER,
				L"CA");

			if (hSystemStore == NULL)
			{
				int a = 1;
			}

			pTargetCert = CertFindCertificateInStore(
				hSystemStore,           // Store handle.
				MY_ENCODING_TYPE,       // Encoding type.
				0,                      // Not used.
				CERT_FIND_SUBJECT_STR_A,// Find type. Find a string in the 
				// certificate's subject.
				szSubjectName,          // The string to be searched for.
				pTargetCert);


			pTargetCertInfo = pTargetCert->pCertInfo;

			switch (CertVerifyTimeValidity(
				NULL,               // Use current time.
				pTargetCertInfo))   // Pointer to CERT_INFO.
			{
			case -1:
			{
				printf("Certificate is not valid yet. \n");
				break;
			}
			case 1:
			{
				printf("Certificate is expired. \n");
				break;
			}
			case 0:
			{
				printf("Certificate's time is valid. \n");
				break;
			}
			};

			// Clean up memory and quit.

			if (pTargetCert)
			{
				CertFreeCertificateContext(pTargetCert);
			}

			if (hSystemStore)
			{
				if (!CertCloseStore(hSystemStore, CERT_CLOSE_STORE_CHECK_FLAG))
				{
					printf("Could not close the certificate store");
				}
			}

			printf("The certificate has been freed and the store closed. \n");
			printf("The certificate verification program ran to completion "
				"without error. \n");
		}

	};
}