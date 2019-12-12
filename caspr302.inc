/* caspr302.inc

   november 22, 2000

   SCIPS.com, Inc.

   include file to print mso dec page and forms

   ------------------------------------------------------
   7/25/2002    REL             changed program to use scipscontrol to give the
                                name of the .sd program to run in order to print
                                the correct auto id cards for the correct
                                company
   09/06/2002  REL              Changed program to use scipscontrol to get the
                                mailer page to use.
   09/27/2002  REL              added code so MCA811 will print when needed.
   02/27/2003  REL              added tpd4 to print on new business
   02/27/2003  REL              added tpd5 to print on Renewal business
   03/05/2003  REL              added scipscontrol value for tpd5
   03/15/2010  JLB              added code to print static forms
*/

Deletefile(L_MACRO)

access sfpname, 
   set sfpname:policy_no = g_policy_no,
       sfpname:pol_year = g_year,
       sfpname:end_sequence = g_end_sequence, generic

access SFSLINE, 
   Set SFSLINE:COMPANY_ID       = SFPNAME:COMPANY_ID,
       SFSLINE:LINE_OF_BUSINESS = L_LINE_OF_BUSINESS,
       SFSLINE:LOB_SUBLINE      = "00", Exact

access SFSLINE_alias, 
   Set SFSLINE_alias:COMPANY_ID       = SFPNAME:COMPANY_ID,
       SFSLINE_alias:LINE_OF_BUSINESS = SFPNAME:LINE_OF_BUSINESS,
       SFSLINE_alias:LOB_SUBLINE      = "00", Exact

access capvehicle, 
   set capvehicle:policy_no = g_policy_no,
       capvehicle:pol_year = g_year,
       capvehicle:end_sequence = g_end_sequence, generic

L_LOB_CODE         = SFSLINE:LOB_CODE
i_state            = capvehicle:state
l_line_of_business = capvehicle:line_of_business
l_company_id       = sfpname:company_id

Access SCIPSCONTROL,
   Set SCIPSCONTROL:COMPANY_ID = SFPNAME:COMPANY_ID,
       SCIPSCONTROL:STATE      = L_STATE, -- change from i_state
       SCIPSCONTROL:LOB_CODE   = L_LOB_CODE, Exact

/*I added the scipscontrol:insured_mailer_page = "" to make sure that
  it only does the mailing by agent when the the
  scipscontrol:insured_mailer_page = ""
*/

If SFPPRINT1:NEW_AGENT_NO = "Y" And
   L_COPIES_PRINTED = 1 And 
   SCIPSCONTROL:INSURED_MAILER_PAGE = "" and
   sfsline_alias:lob_code not one of "CPORTFOLIO" Then
  {
    l_REPORT = "sfpmailer"
    Include "sfsmacro1.inc"
  }
Else  --print the insured mailer page
  If L_COPIES_PRINTED = 1 And 
     SCIPSCONTROL:INSURED_MAILER_PAGE <> "" and
     sfsline_alias:lob_code not one of "CPORTFOLIO" Then
    {
      L_REPORT = Trun(SCIPSCONTROL:INSURED_MAILER_PAGE)
      Include "sfsmacro1.inc"
    }

-- proxy
If L_COPIES_PRINTED = 1 And
   G_END_SEQUENCE = 0 And
   SCIPSCONTROL:PROXY_TO_PRINT <> "" Then
    Begin
    L_REPORT = SCIPSCONTROL:PROXY_TO_PRINT
    Include "sfsmacro1.inc"
    End

-- ID card

if L_COPIES_PRINTED = 1 and
   sfpmaster:trans_code one of 10, 14 then
--   l_extend_hired_nonowned <> "Y" and
--   l_delete_hired_nonowned <> "Y" Then
    Begin
    L_REPORT = SCIPSCONTROL:CA_ID_CARD_TO_USE

	    Include "sfsmacro2.inc"
	    End
if L_COPIES_PRINTED = 1 and
   sfscompany:boiler_rating = 4 and
   sfpmaster:trans_code one of 12, 13 then
--   l_extend_hired_nonowned = "N" and
--   l_delete_hired_nonowned = "N" Then
    Begin
      L_REPORT = SCIPSCONTROL:CA_ID_CARD_TO_USE
-- sfsmacro2 was created for id cards to only pass printer number
-- it does not need copy name
    Include "sfsmacro2.inc"
    End

If SFPMASTER:TRANS_CODE One Of 10 and
   sfpmaster:terrorism_premium <> 0 and
   scipscontrol:dont_print_tpd4_new_business one of 0 Then  --terrorism
  Begin
    L_REPORT = "tpd4"
    Include "sfsmacro2.inc"
  End

If SFPMASTER:TRANS_CODE One Of 14 and
   sfpmaster:terrorism_premium <> 0 Then  --terrorism
  Begin
    L_REPORT = "tpd5"
    Include "sfsmacro2.inc"
  End

-- wh16 endorsement changes

If ((SFPMASTER:TRANS_CODE = 12 Or
   SfPMASTER:TRANS_CODE = 13) and
   sfsline_alias:lob_code not one of "CPORTFOLIO") Then
    Begin
    L_REPORT = "hopsc16"
    Include "sfsmacro1.inc"
    End

-- dec page
L_REPORT = SCIPSCONTROL:DEC_PAGE_TO_USE
Include "sfsmacro1.inc"

    L_REPORT = "casdrivers"
    Include "sfsmacro1.inc"
L_report = ""
-- New Jersey mandatory form

L_STATE            = CAPVEHICLE:STATE
L_LINE_OF_BUSINESS = CAPVEHICLE:LINE_OF_BUSINESS
L_COMPANY_ID       = CAPVEHICLE:COMPANY_ID
L_PARAMETER        = "casmca301.par"

L_CODE = "MCA820"
L_FORM = 0

Include "sfsbpend1.inc"
If L_STATE = 29 And
   G_END_SEQUENCE = 0 And
   L_COPIES_PRINTED = 1 And
   L_PRINT = 1 Then
  Begin
    Include "sfsmacro1.inc"
  End

L_CODE = "MCA825"
L_FORM = 0

Include "sfsbpend1.inc"  --assigns l_report for sfsmacro1.inc

If L_STATE = 29 And
   L_PRINT = 1 Then
    Begin
    Include "sfsmacro1.inc"
    End

L_CODE = "MCA322"
L_FORM = 0

Include "sfsbpend1.inc"
If l_state = 29 And
   l_print = 1 then
    Begin
    Include "sfsmacro1.inc"
    End

if sfpname:company_id one of "DELOS", "GGUND" and
   scipscontrol:use_new_print_program = 1 then
                      --they use mandatory form so I need to access up to the form first
Begin
  access sfsend, 
     set sfsend:company_id       = l_company_id,
         sfsend:state            = l_state,
         sfsend:line_of_business = l_line_of_business,
         sfsend:form             = l_form, generic

While sfsend:company_id       = l_company_id and
      sfsend:state            = l_state and
      sfsend:line_of_business = l_line_of_business and
      sfsend:form             = l_form
  begin
    if ((sfpname:eff_date >= sfsend:eff_date and
        (sfpname:eff_date <= sfsend:exp_date or
                             sfsend:exp_date = 0)) and
                             sfsend:print_form = 1) then
      begin
        If sfsend:print_form = 1 then
          {
            l_print           = sfsend:print_form
            l_form_edition    = sfsend:form_edition
            l_form_print      = sfsend:form_print
            l_edition_print   = sfsend:edition_print
            l_report          = sfsend:form_file_name
            L_CODE            = SFSEND:CODE/raw
            L_REPORT          = sfsend:form_file_name 
            l_use_batch_macro = "Y" 
            if trun(L_COPY_NAME) one of  "Agent Copy", "Agents Copy","Original","Copy" and
               sfsend:pcl_doc_was_converted = 1 then
              {
                Writefile(L_MACRO,"cli joinit %s%s.pcl %s<nl>",trun(scipscontrol:STATIC_FORM_DIR_LOCATION),Trun(L_REPORT),Trun(l_policy_management_dec_copy))
              }
            else
              {
                do ENDORSEMENT_PRINT1
              }
          }
        else
          { 
            L_PRINT           = 0
            L_FORM_EDITION    = ""
            L_FORM_PRINT      = ""
            L_EDITION_PRINT   = "" 
            l_report          = ""
            L_USE_BATCH_MACRO = "N"
          }
     End
    next sfsend
  end---end of sfsend
END---end of mandatory procedure


If SFSCOMPANY:PRINT_ENDORSEMENTS = "Y" Then
  Begin
    access SFPEND1, 
       Set SFPEND1:POLICY_NO    = G_POLICY_NO,
           SFPEND1:POL_YEAR     = G_YEAR,
           SFPEND1:END_SEQUENCE = G_END_SEQUENCE, Approximate

    while SFPEND1:POLICY_NO    = G_POLICY_NO and
          SFPEND1:POL_YEAR     = G_YEAR and
          SFPEND1:eND_SEQUENCE = G_END_SEQUENCE
      {
         If SFPEND1:OK_PRINT              = "Y" Or
            CASDEFAULT:DONT_PRINT_OPT_END = 0 or
            sfpend1:end_sequence = 0 Then
          {
            l_code             = sfpend1:code
            L_PREMIUM          = SFPEND1:PREMIUM
            l_line_of_business = capvehicle:line_of_business
            l_company_id       = sfpname:company_id
            l_state            = capvehicle:state
            do ACCESS_SFSOPTEND
            do access_sfsoptend1
            do add_and_check_print_array
          }
        Next SFPEND1
      }
  End

Writefile(L_MACRO,"cli rm %s",Trun(L_MACRO))
Closefile(L_MACRO)
If SFSCOMPANY:DEBUG_MODE = 0 Then
  {
    Proc("cli cq %s",Trun(L_MACRO))/noconsole
  }
Else
  {
    Proc("cli cq %s",Trun(L_MACRO))
  }
