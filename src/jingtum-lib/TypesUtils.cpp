#include "pch.h"
#include "stdafx.h"
#include "TypesUtils.h"

boost::property_tree::ptree FIELDS_MAP() {
	boost::property_tree::ptree fm;

	boost::property_tree::ptree a;
	a.put("1", "LedgerEntryType");
	a.put("2", "TransactionType");
	fm.put_child("1", a);

	a.clear();
	a.put("2", "Flags");
	a.put("3", "SourceTag");
	a.put("4", "Sequence");
	a.put("5", "PreviousTxnLgrSeq");
	a.put("6", "LedgerSequence");
	a.put("7", "CloseTime");
	a.put("8", "ParentCloseTime");
	a.put("9", "SigningTime");
	a.put("10", "Expiration");
	a.put("11", "TransferRate");
	a.put("12", "WalletSize");
	a.put("13", "OwnerCount");
	a.put("14", "DestinationTag");
	a.put("15", "Timestamp");
	a.put("16", "HighQualityIn");
	a.put("17", "HighQualityOut");
	a.put("18", "LowQualityIn");
	a.put("19", "LowQualityOut");
	a.put("20", "QualityIn");
	a.put("21", "QualityOut");
	a.put("22", "StampEscrow");
	a.put("23", "BondAmount");
	a.put("24", "LoadFee");
	a.put("25", "OfferSequence");
	a.put("26", "FirstLedgerSequence");
	a.put("27", "LastLedgerSequence");
	a.put("28", "TransactionIndex");
	a.put("29", "OperationLimit");
	a.put("30", "ReferenceFeeUnits");
	a.put("31", "ReserveBase");
	a.put("32", "ReserveIncrement");
	a.put("33", "SetFlag");
	a.put("34", "ClearFlag");
	a.put("35", "RelationType");
	a.put("36", "Method");
	a.put("37", "AppType");
	a.put("39", "Contracttype");
	fm.put_child("2", a);

	a.clear();
	a.put("1", "IndexNext");
	a.put("2", "IndexPrevious");
	a.put("3", "BookNode");
	a.put("4", "OwnerNode");
	a.put("5", "BaseFee");
	a.put("6", "ExchangeRate");
	a.put("7", "LowNode");
	a.put("8", "HighNode");
	a.put("9", "OfferFeeRateNum");
	a.put("10", "OfferFeeRateDen");
	fm.put_child("3", a);

	a.clear();
	a.put("1", "EmailHash");
	fm.put_child("4", a);

	a.clear();
	a.put("1", "LedgerHash");
	a.put("2", "ParentHash");
	a.put("3", "TransactionHash");
	a.put("4", "AccountHash");
	a.put("5", "PreviousTxnID");
	a.put("6", "LedgerIndex");
	a.put("7", "WalletLocator");
	a.put("8", "RootIndex");
	a.put("9", "AccountTxnID");
	a.put("16", "BookDirectory");
	a.put("17", "InvoiceID");
	a.put("18", "Nickname");
	a.put("19", "Amendment");
	a.put("20", "TicketID");
	fm.put_child("5", a);

	a.clear();
	a.put("1", "Amount");
	a.put("2", "Balance");
	a.put("3", "LimitAmount");
	a.put("4", "TakerPays");
	a.put("5", "TakerGets");
	a.put("6", "LowLimit");
	a.put("7", "HighLimit");
	a.put("8", "Fee");
	a.put("9", "SendMax");
	a.put("16", "MinimumOffer");
	a.put("17", "JingtumEscrow");
	a.put("18", "DeliveredAmount");
	fm.put_child("6", a);

	a.clear();
	a.put("1", "PublicKey");
	a.put("2", "MessageKey");
	a.put("3", "SigningPubKey");
	a.put("4", "TxnSignature");
	a.put("5", "Generator");
	a.put("6", "Signature");
	a.put("7", "Domain");
	a.put("8", "FundCode");
	a.put("9", "RemoveCode");
	a.put("10", "ExpireCode");
	a.put("11", "CreateCode");
	a.put("12", "MemoType");
	a.put("13", "MemoData");
	a.put("14", "MemoFormat");
	a.put("15", "Payload");
	a.put("17", "ContractMethod");
	a.put("18", "Parameter");	
	fm.put_child("7", a);

	a.clear();
	a.put("1", "Account");
	a.put("2", "Owner");
	a.put("3", "Destination");
	a.put("4", "Issuer");
	a.put("7", "Target");
	a.put("8", "RegularKey");
	fm.put_child("8", a);

	a.clear();
	a.put("1", "none");
	a.put("2", "TransactionMetaData");
	a.put("3", "CreatedNode");
	a.put("4", "DeletedNode");
	a.put("5", "ModifiedNode");
	a.put("6", "PreviousFields");
	a.put("7", "FinalFields");
	a.put("8", "NewFields");
	a.put("9", "TemplateEntry");
	a.put("10", "Memo");
	a.put("11", "Arg");
	fm.put_child("14", a);

	a.clear();
	a.put("1", "none");
	a.put("2", "SigningAccounts");
	a.put("3", "TxnSignatures");
	a.put("4", "Signatures");
	a.put("5", "Template");
	a.put("6", "Necessary");
	a.put("7", "Sufficient");
	a.put("8", "AffectedNodes");
	a.put("9", "Memos");
	a.put("10", "Args");
	fm.put_child("15", a);

	a.clear();
	a.put("1", "CloseResolution");
	a.put("2", "TemplateEntryType");
	a.put("3", "TransactionResult");
	fm.put_child("16", a);

	a.clear();
	a.put("1", "TakerPaysCurrency");
	a.put("2", "TakerPaysIssuer");
	a.put("3", "TakerGetsCurrency");
	a.put("4", "TakerGetsIssuer");
	fm.put_child("17", a);

	a.clear();
	a.put("1", "Paths");
	fm.put_child("18", a);

	a.clear();
	a.put("1", "Indexes");
	a.put("2", "Hashes");
	a.put("3", "Amendments");
	fm.put_child("19", a);

	return fm;
}

boost::property_tree::ptree INVERSE_FIELDS_MAP() {
	boost::property_tree::ptree ifm;

	boost::property_tree::ptree a, a1, a2;

	a1.put_value(1); a2.put_value(1); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("LedgerEntryType", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(1); a2.put_value(2); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("TransactionType", a);

	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(2); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Flags", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(3); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("SourceTag", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(4); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Sequence", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(5); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("PreviousTxnLgrSeq", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(6); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("LedgerSequence", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(7); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("CloseTime", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(8); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("ParentCloseTime", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(9); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("SigningTime", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(10); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Expiration", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(11); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("TransferRate", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(12); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("WalletSize", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(13); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("OwnerCount", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(14); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("DestinationTag", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(15); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Timestamp", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(16); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("HighQualityIn", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(17); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("HighQualityOut", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(18); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("LowQualityIn", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(19); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("LowQualityOut", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(20); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("QualityIn", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(21); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("QualityOut", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(22); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("StampEscrow", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(23); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("BondAmount", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(24); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("LoadFee", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(25); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("OfferSequence", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(26); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("FirstLedgerSequence", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(27); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("LastLedgerSequence", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(28); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("TransactionIndex", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(29); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("OperationLimit", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(30); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("ReferenceFeeUnits", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(31); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("ReserveBase", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(32); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("ReserveIncrement", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(33); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("SetFlag", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(34); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("ClearFlag", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(35); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("RelationType", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(36); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Method", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(37); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("AppType", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(2); a2.put_value(39); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Contracttype", a);

	a1.clear(); a2.clear(); a.clear();	a1.put_value(3); a2.put_value(1); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("IndexNext", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(3); a2.put_value(2); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("IndexPrevious", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(3); a2.put_value(3); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("BookNode", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(3); a2.put_value(4); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("OwnerNode", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(3); a2.put_value(5); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("BaseFee", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(3); a2.put_value(6); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("ExchangeRate", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(3); a2.put_value(7); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("LowNode", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(3); a2.put_value(8); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("HighNode", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(3); a2.put_value(9); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("OfferFeeRateNum", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(3); a2.put_value(10); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("OfferFeeRateDen", a);

	a1.clear(); a2.clear(); a.clear();	a1.put_value(4); a2.put_value(1); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("EmailHash", a);

	a1.clear(); a2.clear(); a.clear();	a1.put_value(5); a2.put_value(1); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("LedgerHash", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(5); a2.put_value(2); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("ParentHash", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(5); a2.put_value(3); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("TransactionHash", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(5); a2.put_value(4); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("AccountHash", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(5); a2.put_value(5); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("PreviousTxnID", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(5); a2.put_value(6); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("LedgerIndex", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(5); a2.put_value(7); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("WalletLocator", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(5); a2.put_value(8); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("RootIndex", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(5); a2.put_value(9); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("AccountTxnID", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(5); a2.put_value(16); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("BookDirectory", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(5); a2.put_value(17); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("InvoiceID", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(5); a2.put_value(18); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Nickname", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(5); a2.put_value(19); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Amendment", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(5); a2.put_value(20); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("TicketID", a);

	a1.clear(); a2.clear(); a.clear();	a1.put_value(6); a2.put_value(1); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Amount", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(6); a2.put_value(2); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Balance", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(6); a2.put_value(3); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("LimitAmount", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(6); a2.put_value(4); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("TakerPays", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(6); a2.put_value(5); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("TakerGets", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(6); a2.put_value(6); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("LowLimit", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(6); a2.put_value(7); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("HighLimit", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(6); a2.put_value(8); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Fee", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(6); a2.put_value(9); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("SendMax", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(6); a2.put_value(16); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("MinimumOffer", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(6); a2.put_value(17); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("JingtumEscrow", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(6); a2.put_value(18); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("DeliveredAmount", a);

	a1.clear(); a2.clear(); a.clear();	a1.put_value(7); a2.put_value(1); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("PublicKey", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(7); a2.put_value(2); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("MessageKey", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(7); a2.put_value(3); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("SigningPubKey", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(7); a2.put_value(4); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("TxnSignature", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(7); a2.put_value(5); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Generator", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(7); a2.put_value(6); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Signature", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(7); a2.put_value(7); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Domain", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(7); a2.put_value(8); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("FundCode", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(7); a2.put_value(9); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("RemoveCode", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(7); a2.put_value(10); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("ExpireCode", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(7); a2.put_value(11); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("CreateCode", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(7); a2.put_value(12); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("MemoType", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(7); a2.put_value(13); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("MemoData", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(7); a2.put_value(14); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("MemoFormat", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(7); a2.put_value(15); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Payload", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(7); a2.put_value(17); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("ContractMethod", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(7); a2.put_value(18); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Parameter", a);

	a1.clear(); a2.clear(); a.clear();	a1.put_value(8); a2.put_value(1); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Account", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(8); a2.put_value(2); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Owner", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(8); a2.put_value(3); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Destination", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(8); a2.put_value(4); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Issuer", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(8); a2.put_value(7); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Target", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(8); a2.put_value(7); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("RegularKey", a);
	
	a1.clear(); a2.clear(); a.clear();	a1.put_value(14); a2.put_value(2); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("TransactionMetaData", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(14); a2.put_value(3); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("CreatedNode", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(14); a2.put_value(4); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("DeletedNode", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(14); a2.put_value(5); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("ModifiedNode", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(14); a2.put_value(6); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("PreviousFields", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(14); a2.put_value(7); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("FinalFields", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(14); a2.put_value(8); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("NewFields", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(14); a2.put_value(9); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("TemplateEntry", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(14); a2.put_value(10); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Memo", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(14); a2.put_value(11); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Arg", a);

	a1.clear(); a2.clear(); a.clear();	a1.put_value(15); a2.put_value(1); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("undefined", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(15); a2.put_value(2); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("SigningAccounts", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(15); a2.put_value(3); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("TxnSignatures", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(15); a2.put_value(4); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Signatures", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(15); a2.put_value(5); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Template", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(15); a2.put_value(6); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Necessary", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(15); a2.put_value(7); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Sufficient", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(15); a2.put_value(8); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("AffectedNodes", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(15); a2.put_value(9); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Memos", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(15); a2.put_value(10); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Args", a);

	a1.clear(); a2.clear(); a.clear();	a1.put_value(16); a2.put_value(1); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("CloseResolution", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(16); a2.put_value(2); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("TemplateEntryType", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(16); a2.put_value(3); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("TransactionResult", a);

	a1.clear(); a2.clear(); a.clear();	a1.put_value(17); a2.put_value(1); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("TakerPaysCurrency", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(17); a2.put_value(2); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("TakerPaysIssuer", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(17); a2.put_value(3); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("TakerGetsCurrency", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(17); a2.put_value(4); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("TakerGetsIssuer", a);

	a1.clear(); a2.clear(); a.clear();	a1.put_value(18); a2.put_value(1); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Paths", a);

	a1.clear(); a2.clear(); a.clear();	a1.put_value(19); a2.put_value(1); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Indexes", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(19); a2.put_value(2); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Hashes", a);
	a1.clear(); a2.clear(); a.clear();	a1.put_value(19); a2.put_value(3); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	ifm.put_child("Amendments", a);

	return ifm;
}

boost::property_tree::ptree base() {
	boost::property_tree::ptree ba;
	boost::property_tree::ptree a, a1, a2, b, b1, b2, c, c1, c2, d, d1, d2, e, e1, e2, f, f1, f2, g, g1, g2, h, h1, h2, j, j1, j2, k, k1, k2;

	a1.put_value("TransactionType");    a2.put_value(RE); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	b1.put_value("Flags");              b2.put_value(OP); b.push_back(std::make_pair("", b1)); b.push_back(std::make_pair("", b2));
	c1.put_value("SourceTag");          c2.put_value(OP); c.push_back(std::make_pair("", c1)); c.push_back(std::make_pair("", c2));
	d1.put_value("LastLedgerSequence"); d2.put_value(OP); d.push_back(std::make_pair("", d1)); d.push_back(std::make_pair("", d2));
	e1.put_value("Account");            e2.put_value(RE); e.push_back(std::make_pair("", e1)); e.push_back(std::make_pair("", e2));
	f1.put_value("Sequence");           f2.put_value(OP); f.push_back(std::make_pair("", f1)); f.push_back(std::make_pair("", f2));
	g1.put_value("Fee");                g2.put_value(RE); g.push_back(std::make_pair("", g1)); g.push_back(std::make_pair("", g2));
	h1.put_value("OperationLimit");     h2.put_value(OP); h.push_back(std::make_pair("", h1)); h.push_back(std::make_pair("", h2));
	j1.put_value("SigningPubKey");      j2.put_value(OP); j.push_back(std::make_pair("", j1)); j.push_back(std::make_pair("", j2));
	k1.put_value("TxnSignature");       k2.put_value(OP); k.push_back(std::make_pair("", k1)); k.push_back(std::make_pair("", k2));

	ba.push_back(std::make_pair("", a));	ba.push_back(std::make_pair("", b));	ba.push_back(std::make_pair("", c));
	ba.push_back(std::make_pair("", d));	ba.push_back(std::make_pair("", e));	ba.push_back(std::make_pair("", f));
	ba.push_back(std::make_pair("", g));	ba.push_back(std::make_pair("", h));	ba.push_back(std::make_pair("", j));
	ba.push_back(std::make_pair("", k));

	return ba;
}

boost::property_tree::ptree TRANSACTION_TYPES(boost::property_tree::ptree base) {

	boost::property_tree::ptree TT;

	boost::property_tree::ptree bb1, bb2, bb3, bb4, bb5, bb6, bb7, bb8, bb9, bb10, bb11, bb12;
	bb1 = base; bb2 = base; bb3 = base; bb4 = base; bb5 = base; bb6 = base; bb7 = base; bb8 = base; bb9 = base; bb10 = base; bb11 = base; bb12 = base;

	// AccountSet
	boost::property_tree::ptree a1, a11, a12, b1, b11, b12, c1, c11, c12, d1, d11, d12, e1, e11, e12, f1, f11, f12;
	a11.put_value("EmailHash");        a12.put_value(OP); a1.push_back(std::make_pair("", a11)); a1.push_back(std::make_pair("", a12));
	b11.put_value("WalletLocator");    b12.put_value(OP); b1.push_back(std::make_pair("", b11)); b1.push_back(std::make_pair("", b12));
	c11.put_value("WalletSize");       c12.put_value(OP); c1.push_back(std::make_pair("", c11)); c1.push_back(std::make_pair("", c12));
	d11.put_value("MessageKey");       d12.put_value(OP); d1.push_back(std::make_pair("", d11)); d1.push_back(std::make_pair("", d12));
	e11.put_value("Domain");           e12.put_value(OP); e1.push_back(std::make_pair("", e11)); e1.push_back(std::make_pair("", e12));
	f11.put_value("TransferRate");     f12.put_value(OP); f1.push_back(std::make_pair("", f11)); f1.push_back(std::make_pair("", f12));
	bb1.push_back(std::make_pair("", a1));	bb1.push_back(std::make_pair("", b1));	bb1.push_back(std::make_pair("", c1));
	bb1.push_back(std::make_pair("", d1));	bb1.push_back(std::make_pair("", e1));	bb1.push_back(std::make_pair("", f1));
	boost::property_tree::ptree t1;
	t1.put_value(3);
	bb1.push_front(std::make_pair("", t1));
	TT.put_child("AccountSet", bb1);

	// TrustSet
	boost::property_tree::ptree a2, a21, a22, b2, b21, b22, c2, c21, c22;
	a21.put_value("LimitAmount");           a22.put_value(OP); a2.push_back(std::make_pair("", a21)); a2.push_back(std::make_pair("", a22));
	b21.put_value("QualityIn");             b22.put_value(OP); b2.push_back(std::make_pair("", b21)); b2.push_back(std::make_pair("", b22));
	c21.put_value("QualityOut");            c22.put_value(OP); c2.push_back(std::make_pair("", c21)); c2.push_back(std::make_pair("", c22));
	bb2.push_back(std::make_pair("", a2));	bb2.push_back(std::make_pair("", b2));	bb2.push_back(std::make_pair("", c2));
	boost::property_tree::ptree t2;
	t2.put_value(20);
	bb2.push_front(std::make_pair("", t2));
	TT.put_child("TrustSet", bb2);

	// RelationSet
	boost::property_tree::ptree a3, a31, a32, b3, b31, b32, c3, c31, c32;
	a31.put_value("Target");                a32.put_value(RE); a3.push_back(std::make_pair("", a31)); a3.push_back(std::make_pair("", a32));
	b31.put_value("RelationType");          b32.put_value(RE); b3.push_back(std::make_pair("", b31)); b3.push_back(std::make_pair("", b32));
	c31.put_value("LimitAmount");           c32.put_value(OP); c3.push_back(std::make_pair("", c31)); c3.push_back(std::make_pair("", c32));
	bb3.push_back(std::make_pair("", a3));	bb3.push_back(std::make_pair("", b3));	bb3.push_back(std::make_pair("", c3));
	boost::property_tree::ptree t3;
	t3.put_value(21);
	bb3.push_front(std::make_pair("", t3));
	TT.put_child("RelationSet", bb3);

	// RelationDel
	boost::property_tree::ptree a4, a41, a42, b4, b41, b42, c4, c41, c42;
	a41.put_value("Target");                a42.put_value(RE); a4.push_back(std::make_pair("", a41)); a4.push_back(std::make_pair("", a42));
	b41.put_value("RelationType");          b42.put_value(RE); b4.push_back(std::make_pair("", b41)); b4.push_back(std::make_pair("", b42));
	c41.put_value("LimitAmount");           c42.put_value(OP); c4.push_back(std::make_pair("", c41)); c4.push_back(std::make_pair("", c42));
	bb4.push_back(std::make_pair("", a4));	bb4.push_back(std::make_pair("", b4));	bb4.push_back(std::make_pair("", c4));
	boost::property_tree::ptree t4;
	t4.put_value(22);
	bb4.push_front(std::make_pair("", t4));
	TT.put_child("RelationDel", bb4);

	// OfferCreate
	boost::property_tree::ptree a5, a51, a52, b5, b51, b52, c5, c51, c52;
	a51.put_value("TakerPays");             a52.put_value(RE); a5.push_back(std::make_pair("", a51)); a5.push_back(std::make_pair("", a52));
	b51.put_value("TakerGets");             b52.put_value(RE); b5.push_back(std::make_pair("", b51)); b5.push_back(std::make_pair("", b52));
	c51.put_value("Expiration");            c52.put_value(OP); c5.push_back(std::make_pair("", c51)); c5.push_back(std::make_pair("", c52));
	bb5.push_back(std::make_pair("", a5));	bb5.push_back(std::make_pair("", b5));	bb5.push_back(std::make_pair("", c5));
	boost::property_tree::ptree t5;
	t5.put_value(7);
	bb5.push_front(std::make_pair("", t5));
	TT.put_child("OfferCreate", bb5);

	// OfferCancel
	boost::property_tree::ptree a6, a61, a62;
	a61.put_value("OfferSequence");    a62.put_value(RE); a6.push_back(std::make_pair("", a61)); a6.push_back(std::make_pair("", a62));
	bb6.push_back(std::make_pair("", a6));
	boost::property_tree::ptree t6;
	t6.put_value(8);
	bb6.push_front(std::make_pair("", t6));
	TT.put_child("OfferCancel", bb6);

	// SetRegularKey
	boost::property_tree::ptree a7, a71, a72;
	a71.put_value("RegularKey");    a72.put_value(RE); a7.push_back(std::make_pair("", a71)); a7.push_back(std::make_pair("", a72));
	bb7.push_back(std::make_pair("", a7));
	boost::property_tree::ptree t7;
	t7.put_value(5);
	bb7.push_front(std::make_pair("", t7));
	TT.put_child("SetRegularKey", bb7);

	// Payment
	boost::property_tree::ptree a8, a81, a82, b8, b81, b82, c8, c81, c82, d8, d81, d82, e8, e81, e82, f8, f81, f82;
	a81.put_value("Destination");       a82.put_value(RE); a8.push_back(std::make_pair("", a81)); a8.push_back(std::make_pair("", a82));
	b81.put_value("Amount");            b82.put_value(RE); b8.push_back(std::make_pair("", b81)); b8.push_back(std::make_pair("", b82));
	c81.put_value("SendMax");           c82.put_value(OP); c8.push_back(std::make_pair("", c81)); c8.push_back(std::make_pair("", c82));
	d81.put_value("Paths");             d82.put_value(DE); d8.push_back(std::make_pair("", d81)); d8.push_back(std::make_pair("", d82));
	e81.put_value("InvoiceID");         e82.put_value(OP); e8.push_back(std::make_pair("", e81)); e8.push_back(std::make_pair("", e82));
	f81.put_value("DestinationTag");    f82.put_value(OP); f8.push_back(std::make_pair("", f81)); f8.push_back(std::make_pair("", f82));
	bb8.push_back(std::make_pair("", a8));	bb8.push_back(std::make_pair("", b8));	bb8.push_back(std::make_pair("", c8));
	bb8.push_back(std::make_pair("", d8));	bb8.push_back(std::make_pair("", e8));	bb8.push_back(std::make_pair("", f8));
	boost::property_tree::ptree t8;
	t8.put_value(0);
	bb8.push_front(std::make_pair("", t8));
	TT.put_child("Payment", bb8);

	// Contract
	boost::property_tree::ptree a9, a91, a92, b9, b91, b92, c9, c91, c92, d9, d91, d92, e9, e91, e92, f9, f91, f92, g9, g91, g92, h9, h91, h92;
	a91.put_value("Expiration");        a92.put_value(RE); a9.push_back(std::make_pair("", a91)); a9.push_back(std::make_pair("", a92));
	b91.put_value("BondAmount");        b92.put_value(RE); b9.push_back(std::make_pair("", b91)); b9.push_back(std::make_pair("", b92));
	c91.put_value("StampEscrow");       c92.put_value(RE); c9.push_back(std::make_pair("", c91)); c9.push_back(std::make_pair("", c92));
	d91.put_value("JingtumEscrow");     d92.put_value(RE); d9.push_back(std::make_pair("", d91)); d9.push_back(std::make_pair("", d92));
	e91.put_value("CreateCode");        e92.put_value(OP); e9.push_back(std::make_pair("", e91)); e9.push_back(std::make_pair("", e92));
	f91.put_value("FundCode");          f92.put_value(OP); f9.push_back(std::make_pair("", f91)); f9.push_back(std::make_pair("", f92));
	g91.put_value("RemoveCode");        g92.put_value(OP); g9.push_back(std::make_pair("", g91)); g9.push_back(std::make_pair("", g92));
	h91.put_value("ExpireCode");        h92.put_value(OP); h9.push_back(std::make_pair("", h91)); h9.push_back(std::make_pair("", h92));
	bb9.push_back(std::make_pair("", a9));	bb9.push_back(std::make_pair("", b9));	bb9.push_back(std::make_pair("", c9));
	bb9.push_back(std::make_pair("", d9));	bb9.push_back(std::make_pair("", e9));	bb9.push_back(std::make_pair("", f9));
	bb9.push_back(std::make_pair("", g9));	bb9.push_back(std::make_pair("", h9));
	boost::property_tree::ptree t9;
	t9.put_value(9);
	bb9.push_front(std::make_pair("", t9));
	TT.put_child("Contract", bb9);

	// RemoveContract
	boost::property_tree::ptree aa, aa1, aa2;
	aa1.put_value("Target");    aa2.put_value(RE); aa.push_back(std::make_pair("", aa1)); aa.push_back(std::make_pair("", aa2));
	bb10.push_back(std::make_pair("", aa));
	boost::property_tree::ptree ta;
	ta.put_value(10);
	bb10.push_front(std::make_pair("", ta));
	TT.put_child("RemoveContract", bb10);

	// EnableFeature
	boost::property_tree::ptree ab, ab1, ab2;
	ab1.put_value("Feature");    ab2.put_value(RE); ab.push_back(std::make_pair("", ab1)); ab.push_back(std::make_pair("", ab2));
	bb11.push_back(std::make_pair("", ab));
	boost::property_tree::ptree tb;
	tb.put_value(100);
	bb11.push_front(std::make_pair("", tb));
	TT.put_child("EnableFeature", bb11);

	// SetFee
	boost::property_tree::ptree ac, ac1, ac2, bc, bc1, bc2, cc, cc1, cc2, dc, dc1, dc2, ec, ec1, ec2;
	ac1.put_value("Features");             ac2.put_value(RE); ac.push_back(std::make_pair("", ac1)); ac.push_back(std::make_pair("", ac2));
	bc1.put_value("BaseFee");              bc2.put_value(RE); bc.push_back(std::make_pair("", bc1)); bc.push_back(std::make_pair("", bc2));
	cc1.put_value("ReferenceFeeUnits");    cc2.put_value(RE); cc.push_back(std::make_pair("", cc1)); cc.push_back(std::make_pair("", cc2));
	dc1.put_value("ReserveBase");          dc2.put_value(RE); dc.push_back(std::make_pair("", dc1)); dc.push_back(std::make_pair("", dc2));
	ec1.put_value("ReserveIncrement");     ec2.put_value(RE); ec.push_back(std::make_pair("", ec1)); ec.push_back(std::make_pair("", ec2));
	bb12.push_back(std::make_pair("", ac));	bb12.push_back(std::make_pair("", bc));	bb12.push_back(std::make_pair("", cc));
	bb12.push_back(std::make_pair("", dc));	bb12.push_back(std::make_pair("", ec));
	boost::property_tree::ptree tc;
	tc.put_value(101);
	bb12.push_front(std::make_pair("", tc));
	TT.put_child("SetFee", bb12);

	// ConfigContract
	boost::property_tree::ptree ad, ad1, ad2, bd, bd1, bd2, cd, cd1, cd2, dd, dd1, dd2, ed, ed1, ed2, fd, fd1, fd2, gd, gd1, gd2;
	ad1.put_value("Method");            ad2.put_value(RE); ad.push_back(std::make_pair("", ad1)); ad.push_back(std::make_pair("", ad2));
	bd1.put_value("Payload");           bd2.put_value(OP); bd.push_back(std::make_pair("", bd1)); bd.push_back(std::make_pair("", bd2));
	cd1.put_value("Destination");       cd2.put_value(OP); cd.push_back(std::make_pair("", cd1)); cd.push_back(std::make_pair("", cd2));
	dd1.put_value("Amount");            dd2.put_value(OP); dd.push_back(std::make_pair("", dd1)); dd.push_back(std::make_pair("", dd2));
	ed1.put_value("Contracttype");      ed2.put_value(OP); ed.push_back(std::make_pair("", ed1)); ed.push_back(std::make_pair("", ed2));
	fd1.put_value("ContractMethod");    fd2.put_value(OP); fd.push_back(std::make_pair("", fd1)); fd.push_back(std::make_pair("", fd2));
	gd1.put_value("Args");              gd2.put_value(OP); gd.push_back(std::make_pair("", gd1)); gd.push_back(std::make_pair("", gd2));
	base.push_back(std::make_pair("", ad));	base.push_back(std::make_pair("", bd));	base.push_back(std::make_pair("", cd));
	base.push_back(std::make_pair("", dd));	base.push_back(std::make_pair("", ed));	base.push_back(std::make_pair("", fd));
	base.push_back(std::make_pair("", gd));
	boost::property_tree::ptree td;
	td.put_value(30);
	base.push_front(std::make_pair("", td));
	TT.put_child("ConfigContract", base);

	return TT;
}

boost::property_tree::ptree sleBase() {
	boost::property_tree::ptree ba;
	boost::property_tree::ptree a, a1, a2, b, b1, b2, c, c1, c2;

	a1.put_value("LedgerIndex");        a2.put_value(OP); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	b1.put_value("LedgerEntryType");    b2.put_value(RE); b.push_back(std::make_pair("", b1)); b.push_back(std::make_pair("", b2));
	c1.put_value("Flags");              c2.put_value(RE); c.push_back(std::make_pair("", c1)); c.push_back(std::make_pair("", c2));
	ba.push_back(std::make_pair("", a));	ba.push_back(std::make_pair("", b));	ba.push_back(std::make_pair("", c));

	return ba;
}

boost::property_tree::ptree LEDGER_ENTRY_TYPES(boost::property_tree::ptree sleBase) {
	boost::property_tree::ptree LT;

	boost::property_tree::ptree bb1, bb2, bb3, bb4, bb5, bb6, bb7, bb8, bb9;
	bb1 = sleBase; bb2 = sleBase; bb3 = sleBase; bb4 = sleBase; bb5 = sleBase; bb6 = sleBase; bb7 = sleBase; bb8 = sleBase; bb9 = sleBase;

	// AccountRoot
	boost::property_tree::ptree a1, a11, a12, b1, b11, b12, c1, c11, c12, d1, d11, d12, e1, e11, e12, f1, f11, f12, g1, g11, g12;
	boost::property_tree::ptree h1, h11, h12, j1, j11, j12, k1, k11, k12, m1, m11, m12, n1, n11, n12, p1, p11, p12, q1, q11, q12;
	a11.put_value("Sequence");             a12.put_value(RE); a1.push_back(std::make_pair("", a11)); a1.push_back(std::make_pair("", a12));
	b11.put_value("PreviousTxnLgrSeq");    b12.put_value(RE); b1.push_back(std::make_pair("", b11)); b1.push_back(std::make_pair("", b12));
	c11.put_value("TransferRate");         c12.put_value(OP); c1.push_back(std::make_pair("", c11)); c1.push_back(std::make_pair("", c12));
	d11.put_value("WalletSize");           d12.put_value(OP); d1.push_back(std::make_pair("", d11)); d1.push_back(std::make_pair("", d12));
	e11.put_value("OwnerCount");           e12.put_value(RE); e1.push_back(std::make_pair("", e11)); e1.push_back(std::make_pair("", e12));
	f11.put_value("EmailHash");            f12.put_value(OP); f1.push_back(std::make_pair("", f11)); f1.push_back(std::make_pair("", f12));
	g11.put_value("PreviousTxnID");        g12.put_value(RE); g1.push_back(std::make_pair("", g11)); g1.push_back(std::make_pair("", g12));
	h11.put_value("AccountTxnID");         h12.put_value(OP); h1.push_back(std::make_pair("", h11)); h1.push_back(std::make_pair("", h12));
	j11.put_value("WalletLocator");        j12.put_value(OP); j1.push_back(std::make_pair("", j11)); j1.push_back(std::make_pair("", j12));
	k11.put_value("Balance");              k12.put_value(RE); k1.push_back(std::make_pair("", k11)); k1.push_back(std::make_pair("", k12));
	m11.put_value("MessageKey");           m12.put_value(OP); m1.push_back(std::make_pair("", m11)); m1.push_back(std::make_pair("", m12));
	n11.put_value("Domain");               n12.put_value(OP); n1.push_back(std::make_pair("", n11)); n1.push_back(std::make_pair("", n12));
	p11.put_value("Account");              p12.put_value(RE); p1.push_back(std::make_pair("", p11)); p1.push_back(std::make_pair("", p12));
	q11.put_value("RegularKey");           q12.put_value(OP); q1.push_back(std::make_pair("", q11)); q1.push_back(std::make_pair("", q12));
	bb1.push_back(std::make_pair("", a1));	bb1.push_back(std::make_pair("", b1));	bb1.push_back(std::make_pair("", c1)); bb1.push_back(std::make_pair("", d1));
	bb1.push_back(std::make_pair("", e1));	bb1.push_back(std::make_pair("", f1));  bb1.push_back(std::make_pair("", g1)); bb1.push_back(std::make_pair("", h1));
	bb1.push_back(std::make_pair("", j1));	bb1.push_back(std::make_pair("", k1));	bb1.push_back(std::make_pair("", m1)); bb1.push_back(std::make_pair("", n1));
	bb1.push_back(std::make_pair("", p1));	bb1.push_back(std::make_pair("", q1));
	boost::property_tree::ptree t1;
	t1.put_value(97);
	bb1.push_front(std::make_pair("", t1));
	LT.put_child("AccountRoot", bb1);

	// Contract
	boost::property_tree::ptree a2, a21, a22, b2, b21, b22, c2, c21, c22, d2, d21, d22, e2, e21, e22, f2, f21, f22, g2, g21, g22;
	boost::property_tree::ptree h2, h21, h22, j2, j21, j22, k2, k21, k22, m2, m21, m22, n2, n21, n22;
	a21.put_value("PreviousTxnLgrSeq");     a22.put_value(RE); a2.push_back(std::make_pair("", a21)); a2.push_back(std::make_pair("", a22));
	b21.put_value("Expiration");            b22.put_value(RE); b2.push_back(std::make_pair("", b21)); b2.push_back(std::make_pair("", b22));
	c21.put_value("BondAmount");            c22.put_value(RE); c2.push_back(std::make_pair("", c21)); c2.push_back(std::make_pair("", c22));
	d21.put_value("PreviousTxnID");         d22.put_value(RE); d2.push_back(std::make_pair("", d21)); d2.push_back(std::make_pair("", d22));
	e21.put_value("Balance");               e22.put_value(RE); e2.push_back(std::make_pair("", e21)); e2.push_back(std::make_pair("", e22));
	f21.put_value("FundCode");              f22.put_value(OP); f2.push_back(std::make_pair("", f21)); f2.push_back(std::make_pair("", f22));
	g21.put_value("RemoveCode");            g22.put_value(OP); g2.push_back(std::make_pair("", g21)); g2.push_back(std::make_pair("", g22));
	h21.put_value("ExpireCode");            h22.put_value(OP); h2.push_back(std::make_pair("", h21)); h2.push_back(std::make_pair("", h22));
	j21.put_value("CreateCode");            j22.put_value(OP); j2.push_back(std::make_pair("", j21)); j2.push_back(std::make_pair("", j22));
	k21.put_value("Account");               k22.put_value(RE); k2.push_back(std::make_pair("", k21)); k2.push_back(std::make_pair("", k22));
	m21.put_value("Owner");                 m22.put_value(RE); m2.push_back(std::make_pair("", m21)); m2.push_back(std::make_pair("", m22));
	n21.put_value("Issuer");                n22.put_value(RE); n2.push_back(std::make_pair("", n21)); n2.push_back(std::make_pair("", n22));
	bb2.push_back(std::make_pair("", a2));	bb2.push_back(std::make_pair("", b2));	bb2.push_back(std::make_pair("", c2)); bb2.push_back(std::make_pair("", d2));
	bb2.push_back(std::make_pair("", e2));	bb2.push_back(std::make_pair("", f2));  bb2.push_back(std::make_pair("", g2)); bb2.push_back(std::make_pair("", h2));
	bb2.push_back(std::make_pair("", j2));	bb2.push_back(std::make_pair("", k2));	bb2.push_back(std::make_pair("", m2)); bb2.push_back(std::make_pair("", n2));
	boost::property_tree::ptree t2;
	t2.put_value(99);
	bb2.push_front(std::make_pair("", t2));
	LT.put_child("Contract", bb2);

	// DirectoryNode
	boost::property_tree::ptree a3, a31, a32, b3, b31, b32, c3, c31, c32, d3, d31, d32, e3, e31, e32, f3, f31, f32, g3, g31, g32;
	boost::property_tree::ptree h3, h31, h32, j3, j31, j32, k3, k31, k32;
	a31.put_value("IndexNext");           a32.put_value(OP); a3.push_back(std::make_pair("", a31)); a3.push_back(std::make_pair("", a32));
	b31.put_value("IndexPrevious");       b32.put_value(OP); b3.push_back(std::make_pair("", b31)); b3.push_back(std::make_pair("", b32));
	c31.put_value("ExchangeRate");        c32.put_value(OP); c3.push_back(std::make_pair("", c31)); c3.push_back(std::make_pair("", c32));
	d31.put_value("RootIndex");           d32.put_value(RE); d3.push_back(std::make_pair("", d31)); d3.push_back(std::make_pair("", d32));
	e31.put_value("Owner");               e32.put_value(OP); e3.push_back(std::make_pair("", e31)); e3.push_back(std::make_pair("", e32));
	f31.put_value("TakerPaysCurrency");   f32.put_value(OP); f3.push_back(std::make_pair("", f31)); f3.push_back(std::make_pair("", f32));
	g31.put_value("TakerPaysIssuer");     g32.put_value(OP); g3.push_back(std::make_pair("", g31)); g3.push_back(std::make_pair("", g32));
	h31.put_value("TakerGetsCurrency");   h32.put_value(OP); h3.push_back(std::make_pair("", h31)); h3.push_back(std::make_pair("", h32));
	j31.put_value("TakerGetsIssuer");     j32.put_value(OP); j3.push_back(std::make_pair("", j31)); j3.push_back(std::make_pair("", j32));
	k31.put_value("Indexes");             k32.put_value(RE); k3.push_back(std::make_pair("", k31)); k3.push_back(std::make_pair("", k32));
	bb3.push_back(std::make_pair("", a3));	bb3.push_back(std::make_pair("", b3));	bb3.push_back(std::make_pair("", c3)); bb3.push_back(std::make_pair("", d3));
	bb3.push_back(std::make_pair("", e3));	bb3.push_back(std::make_pair("", f3));  bb3.push_back(std::make_pair("", g3)); bb3.push_back(std::make_pair("", h3));
	bb3.push_back(std::make_pair("", j3));	bb3.push_back(std::make_pair("", k3));
	boost::property_tree::ptree t3;
	t3.put_value(100);
	bb3.push_front(std::make_pair("", t3));
	LT.put_child("DirectoryNode", bb3);

	// EnabledFeatures
	boost::property_tree::ptree a4, a41, a42;
	a41.put_value("Features");           a42.put_value(RE); a4.push_back(std::make_pair("", a41)); a4.push_back(std::make_pair("", a42));
	bb4.push_back(std::make_pair("", a4));
	boost::property_tree::ptree t4;
	t4.put_value(102);
	bb4.push_front(std::make_pair("", t4));
	LT.put_child("EnabledFeatures", bb4);

	// FeeSettings
	boost::property_tree::ptree a5, a51, a52, b5, b51, b52, c5, c51, c52, d5, d51, d52, e5, e51, e52;
	a51.put_value("ReferenceFeeUnits");       a52.put_value(RE); a5.push_back(std::make_pair("", a51)); a5.push_back(std::make_pair("", a52));
	b51.put_value("ReserveBase");             b52.put_value(RE); b5.push_back(std::make_pair("", b51)); b5.push_back(std::make_pair("", b52));
	c51.put_value("ReserveIncrement");        c52.put_value(RE); c5.push_back(std::make_pair("", c51)); c5.push_back(std::make_pair("", c52));
	d51.put_value("BaseFee");                 d52.put_value(RE); d5.push_back(std::make_pair("", d51)); d5.push_back(std::make_pair("", d52));
	e51.put_value("LedgerIndex");             e52.put_value(OP); e5.push_back(std::make_pair("", e51)); e5.push_back(std::make_pair("", e52));
	bb5.push_back(std::make_pair("", a5));	bb5.push_back(std::make_pair("", b5));	bb5.push_back(std::make_pair("", c5)); bb5.push_back(std::make_pair("", d5));
	bb5.push_back(std::make_pair("", e5));
	boost::property_tree::ptree t5;
	t5.put_value(115);
	bb5.push_front(std::make_pair("", t5));
	LT.put_child("FeeSettings", bb5);

	// GeneratorMap
	boost::property_tree::ptree a6, a61, a62;
	a61.put_value("Generator");           a62.put_value(RE); a6.push_back(std::make_pair("", a61)); a6.push_back(std::make_pair("", a62));
	bb6.push_back(std::make_pair("", a6));
	boost::property_tree::ptree t6;
	t6.put_value(103);
	bb6.push_front(std::make_pair("", t6));
	LT.put_child("GeneratorMap", bb6);

	// LedgerHashes
	boost::property_tree::ptree a7, a71, a72, b7, b71, b72, c7, c71, c72, d7, d71, d72, e7, e71, e72, f7, f71, f72;
	a71.put_value("LedgerEntryType");           a72.put_value(RE); a7.push_back(std::make_pair("", a71)); a7.push_back(std::make_pair("", a72));
	b71.put_value("Flags");                     b72.put_value(RE); b7.push_back(std::make_pair("", b71)); b7.push_back(std::make_pair("", b72));
	c71.put_value("FirstLedgerSequence");       c72.put_value(OP); c7.push_back(std::make_pair("", c71)); c7.push_back(std::make_pair("", c72));
	d71.put_value("LastLedgerSequence");        d72.put_value(OP); d7.push_back(std::make_pair("", d71)); d7.push_back(std::make_pair("", d72));
	e71.put_value("LedgerIndex");               e72.put_value(OP); e7.push_back(std::make_pair("", e71)); e7.push_back(std::make_pair("", e72));
	f71.put_value("Hashes");                    f72.put_value(RE); f7.push_back(std::make_pair("", f71)); f7.push_back(std::make_pair("", f72));
	bb7.push_back(std::make_pair("", a7));	bb7.push_back(std::make_pair("", b7));	bb7.push_back(std::make_pair("", c7)); bb7.push_back(std::make_pair("", d7));
	bb7.push_back(std::make_pair("", e7));	bb7.push_back(std::make_pair("", f7));
	boost::property_tree::ptree t7;
	t7.put_value(104);
	bb7.push_front(std::make_pair("", t7));
	LT.put_child("LedgerHashes", bb7);

	// Nickname
	boost::property_tree::ptree a8, a81, a82, b8, b81, b82, c8, c81, c82, d8, d81, d82, e8, e81, e82;
	a81.put_value("LedgerEntryType");       a82.put_value(RE); a8.push_back(std::make_pair("", a81)); a8.push_back(std::make_pair("", a82));
	b81.put_value("Flags");                 b82.put_value(RE); b8.push_back(std::make_pair("", b81)); b8.push_back(std::make_pair("", b82));
	c81.put_value("LedgerIndex");           c82.put_value(OP); c8.push_back(std::make_pair("", c81)); c8.push_back(std::make_pair("", c82));
	d81.put_value("MinimumOffer");          d82.put_value(OP); d8.push_back(std::make_pair("", d81)); d8.push_back(std::make_pair("", d82));
	e81.put_value("Account");               e82.put_value(RE); e8.push_back(std::make_pair("", e81)); e8.push_back(std::make_pair("", e82));
	bb8.push_back(std::make_pair("", a8));	bb8.push_back(std::make_pair("", b8));	bb8.push_back(std::make_pair("", c8)); bb8.push_back(std::make_pair("", d8));
	bb8.push_back(std::make_pair("", e8));
	boost::property_tree::ptree t8;
	t8.put_value(110);
	bb8.push_front(std::make_pair("", t8));
	LT.put_child("Nickname", bb8);

	// Offer
	boost::property_tree::ptree a9, a91, a92, b9, b91, b92, c9, c91, c92, d9, d91, d92, e9, e91, e92, f9, f91, f92, g9, g91, g92;
	boost::property_tree::ptree h9, h91, h92, j9, j91, j92, k9, k91, k92, m9, m91, m92, n9, n91, n92, p9, p91, p92;
	a91.put_value("LedgerEntryType");             a92.put_value(RE); a9.push_back(std::make_pair("", a91)); a9.push_back(std::make_pair("", a92));
	b91.put_value("Flags");                       b92.put_value(RE); b9.push_back(std::make_pair("", b91)); b9.push_back(std::make_pair("", b92));
	c91.put_value("Sequence");                    c92.put_value(RE); c9.push_back(std::make_pair("", c91)); c9.push_back(std::make_pair("", c92));
	d91.put_value("PreviousTxnLgrSeq");           d92.put_value(RE); d9.push_back(std::make_pair("", d91)); d9.push_back(std::make_pair("", d92));
	e91.put_value("Expiration");                  e92.put_value(OP); e9.push_back(std::make_pair("", e91)); e9.push_back(std::make_pair("", e92));
	f91.put_value("BookNode");                    f92.put_value(RE); f9.push_back(std::make_pair("", f91)); f9.push_back(std::make_pair("", f92));
	g91.put_value("OwnerNode");                   g92.put_value(RE); g9.push_back(std::make_pair("", g91)); g9.push_back(std::make_pair("", g92));
	h91.put_value("PreviousTxnID");               h92.put_value(RE); h9.push_back(std::make_pair("", h91)); h9.push_back(std::make_pair("", h92));
	j91.put_value("LedgerIndex");                 j92.put_value(OP); j9.push_back(std::make_pair("", j91)); j9.push_back(std::make_pair("", j92));
	k91.put_value("BookDirectory");               k92.put_value(RE); k9.push_back(std::make_pair("", k91)); k9.push_back(std::make_pair("", k92));
	m91.put_value("TakerPays");                   m92.put_value(RE); m9.push_back(std::make_pair("", m91)); m9.push_back(std::make_pair("", m92));
	n91.put_value("TakerGets");                   n92.put_value(RE); n9.push_back(std::make_pair("", n91)); n9.push_back(std::make_pair("", n92));
	p91.put_value("Account");                     p92.put_value(RE); p9.push_back(std::make_pair("", p91)); p9.push_back(std::make_pair("", p92));
	bb9.push_back(std::make_pair("", a9));	bb9.push_back(std::make_pair("", b9));	bb9.push_back(std::make_pair("", c9)); bb9.push_back(std::make_pair("", d9));
	bb9.push_back(std::make_pair("", e9));	bb9.push_back(std::make_pair("", f9));  bb9.push_back(std::make_pair("", g9)); bb9.push_back(std::make_pair("", h9));
	bb9.push_back(std::make_pair("", j9));	bb9.push_back(std::make_pair("", k9));	bb9.push_back(std::make_pair("", m9)); bb9.push_back(std::make_pair("", n9));
	bb9.push_back(std::make_pair("", p9));
	boost::property_tree::ptree t9;
	t9.put_value(111);
	bb9.push_front(std::make_pair("", t9));
	LT.put_child("Offer", bb9);

	// SkywellState
	boost::property_tree::ptree aa, aa1, aa2, ba, ba1, ba2, ca, ca1, ca2, da, da1, da2, ea, ea1, ea2, fa, fa1, fa2, ga, ga1, ga2;
	boost::property_tree::ptree ha, ha1, ha2, ja, ja1, ja2, ka, ka1, ka2, ma, ma1, ma2, na, na1, na2, pa, pa1, pa2, qa, qa1, qa2;
	aa1.put_value("LedgerEntryType");                aa2.put_value(RE); aa.push_back(std::make_pair("", aa1)); aa.push_back(std::make_pair("", aa2));
	ba1.put_value("Flags");                          ba2.put_value(RE);    ba.push_back(std::make_pair("", ba1)); ba.push_back(std::make_pair("", ba2));
	ca1.put_value("PreviousTxnLgrSeq");              ca2.put_value(RE); ca.push_back(std::make_pair("", ca1)); ca.push_back(std::make_pair("", ca2));
	da1.put_value("HighQualityIn");                  da2.put_value(OP); da.push_back(std::make_pair("", da1)); da.push_back(std::make_pair("", da2));
	ea1.put_value("HighQualityOut");                 ea2.put_value(OP); ea.push_back(std::make_pair("", ea1)); ea.push_back(std::make_pair("", ea2));
	fa1.put_value("LowQualityIn");                   fa2.put_value(OP); fa.push_back(std::make_pair("", fa1)); fa.push_back(std::make_pair("", fa2));
	ga1.put_value("LowQualityOut");                  ga2.put_value(OP); ga.push_back(std::make_pair("", ga1)); ga.push_back(std::make_pair("", ga2));
	ha1.put_value("LowNode");                        ha2.put_value(OP); ha.push_back(std::make_pair("", ha1)); ha.push_back(std::make_pair("", ha2));
	ja1.put_value("HighNode");                       ja2.put_value(OP); ja.push_back(std::make_pair("", ja1)); ja.push_back(std::make_pair("", ja2));
	ka1.put_value("PreviousTxnID");                  ka2.put_value(RE); ka.push_back(std::make_pair("", ka1)); ka.push_back(std::make_pair("", ka2));
	ma1.put_value("LedgerIndex");                    ma2.put_value(OP); ma.push_back(std::make_pair("", ma1)); ma.push_back(std::make_pair("", ma2));
	na1.put_value("Balance");                        na2.put_value(RE); na.push_back(std::make_pair("", na1)); na.push_back(std::make_pair("", na2));
	pa1.put_value("LowLimit");                       pa2.put_value(RE); pa.push_back(std::make_pair("", pa1)); pa.push_back(std::make_pair("", pa2));
	qa1.put_value("HighLimit");                      qa2.put_value(RE); qa.push_back(std::make_pair("", qa1)); qa.push_back(std::make_pair("", qa2));
	sleBase.push_back(std::make_pair("", aa));	sleBase.push_back(std::make_pair("", ba));	sleBase.push_back(std::make_pair("", ca)); sleBase.push_back(std::make_pair("", da));
	sleBase.push_back(std::make_pair("", ea));	sleBase.push_back(std::make_pair("", fa));  sleBase.push_back(std::make_pair("", ga)); sleBase.push_back(std::make_pair("", ha));
	sleBase.push_back(std::make_pair("", ja));	sleBase.push_back(std::make_pair("", ka));	sleBase.push_back(std::make_pair("", ma)); sleBase.push_back(std::make_pair("", na));
	sleBase.push_back(std::make_pair("", pa));	sleBase.push_back(std::make_pair("", qa));
	boost::property_tree::ptree ta;
	ta.put_value(114);
	sleBase.push_front(std::make_pair("", ta));
	LT.put_child("SkywellState", sleBase);

	return LT;
}

boost::property_tree::ptree METADATA() {
	boost::property_tree::ptree ba;
	boost::property_tree::ptree a, a1, a2, b, b1, b2, c, c1, c2;

	a1.put_value("TransactionIndex");        a2.put_value(RE); a.push_back(std::make_pair("", a1)); a.push_back(std::make_pair("", a2));
	b1.put_value("TransactionResult");       b2.put_value(RE); b.push_back(std::make_pair("", b1)); b.push_back(std::make_pair("", b2));
	c1.put_value("AffectedNodes");           c2.put_value(RE); c.push_back(std::make_pair("", c1)); c.push_back(std::make_pair("", c2));
	ba.push_back(std::make_pair("", a));	ba.push_back(std::make_pair("", b));	ba.push_back(std::make_pair("", c));

	return ba;
}