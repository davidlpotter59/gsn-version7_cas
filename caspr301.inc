/* caspr301.inc

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
*/

Deletefile(L_MACRO)

Access SFPMASTER, Set SFPMASTER:POLICY_NO = G_POLICY_NO,
                      SFPMASTER:POL_YEAR = G_YEAR,
                      SFPMASTER:END_SEQUENCE = G_END_SEQUENCE, Exact

Access SFPNAME, Set SFPNAME:POLICY_NO = G_POLICY_NO,
                    SFPNAME:POL_YEAR = G_YEAR,
                    SFPNAME:END_SEQUENCE = G_END_SEQUENCE, Generic

/*add this code for a company that has to mail the policies to the insured
  and to all the mortgagees on a policy
*/
Access SFPLOCATION, Set SFPLOCATION:POLICY_NO    = G_POLICY_NO,
                        SFPLOCATION:POL_YEAR     = G_YEAR,
                        SFPLOCATION:END_SEQUENCE = G_END_SEQUENCE, Generic

Access SFSLINE, Set SFSLINE:COMPANY_ID       = SFPNAME:COMPANY_ID,
                    SFSLINE:LINE_OF_BUSINESS = L_LINE_OF_BUSINESS,
                    SFSLINE:LOB_SUBLINE      = "00", Exact

Access SFSLINE_alias, Set SFSLINE_alias:COMPANY_ID       = SFPNAME:COMPANY_ID,
                          SFSLINE_alias:LINE_OF_BUSINESS = SFPNAME:LINE_OF_BUSINESS,
                          SFSLINE_alias:LOB_SUBLINE      = "00", Exact

Include "swsut101.inc"  --converts the str_state to a 2 digit number uses sfplocation

L_LOB_CODE = SFSLINE:LOB_CODE
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
    L_REPORT = "sfpmailer"
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
   sfpmaster:trans_code one of 10, 14 and
   sfpmaster:delete_hired_nonowned = "N" and 
   sfpmaster:extend_hired_nonowned = "N" Then
    Begin
--To access file to get the correct read on scipscontrol
      Access SFPNAME, Set SFPNAME:POLICY_NO        = G_POLICY_NO,
                          SFPNAME:POL_YEAR         = G_YEAR ,
                          SFPNAME:END_SEQUENCE     = G_END_SEQUENCE, Generic

      Access SFSLINE, Set SFSLINE:COMPANY_ID       = SFPNAME:COMPANY_ID,
                          SFSLINE:LINE_OF_BUSINESS = SFPNAME:LINE_OF_BUSINESS,
                          SFSLINE:LOB_SUBLINE      = "00", Exact

--      Include "swsut101.inc"  --converts the str_state to a 2 digit number
      L_LOB_CODE = SFSLINE:LOB_CODE
      Access SCIPSCONTROL,
             Set SCIPSCONTROL:COMPANY_ID = SFPNAME:COMPANY_ID,
                 SCIPSCONTROL:STATE      = L_STATE, --changed from i_state
                 SCIPSCONTROL:LOB_CODE   = L_LOB_CODE, Exact

      L_REPORT = SCIPSCONTROL:CA_ID_CARD_TO_USE

-- sfsmacro2 was created for id cards to only pass printer number
-- it does not need copy name
    Include "sfsmacro2.inc"
    End

if L_COPIES_PRINTED = 1 and
   sfscompany:boiler_rating = 4 and 
   sfpmaster:trans_code one of 12, 13 and
   sfpmaster:delete_hired_nonowned = "N" and 
   sfpmaster:extend_hired_nonowned = "N" Then
    Begin
--To access file to get the correct read on scipscontrol
      Access SFPNAME, Set SFPNAME:POLICY_NO        = G_POLICY_NO,
                          SFPNAME:POL_YEAR         = G_YEAR ,
                          SFPNAME:END_SEQUENCE     = G_END_SEQUENCE, Generic

      Access SFSLINE, Set SFSLINE:COMPANY_ID       = SFPNAME:COMPANY_ID,
                          SFSLINE:LINE_OF_BUSINESS = SFPNAME:LINE_OF_BUSINESS,
                          SFSLINE:LOB_SUBLINE      = "00", Exact

--      Include "swsut101.inc"  --converts the str_state to a 2 digit number
      L_LOB_CODE = SFSLINE:LOB_CODE
      Access SCIPSCONTROL,
             Set SCIPSCONTROL:COMPANY_ID = SFPNAME:COMPANY_ID,
                 SCIPSCONTROL:STATE      = L_STATE, --changed from i_state
                 SCIPSCONTROL:LOB_CODE   = L_LOB_CODE, Exact

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
   SFPMASTER:TRANS_CODE = 13) and
   sfsline_alias:lob_code not one of "CPORTFOLIO") Then
    Begin
    L_REPORT = "hopsc16"
    Include "sfsmacro1.inc"
    End

-- dec page

L_REPORT = SCIPSCONTROL:DEC_PAGE_TO_USE
Include "sfsmacro1.inc"

--If --l_COPIES_PRINTED = 1 or
--   l_copy_name = "Home Office"  then
    Begin
    L_REPORT = "casdrivers"
    Include "sfsmacro1.inc"
    End

-- New Jersey mandatory form

L_STATE = CAPVEHICLE:STATE
L_LINE_OF_BUSINESS = CAPVEHICLE:LINE_OF_BUSINESS
L_COMPANY_ID = CAPVEHICLE:COMPANY_ID
L_PARAMETER = "casmca301.par"

L_CODE = "MCA820"
L_FORM = 0

Include "sfsbpend1.inc"

If L_STATE = 29 And
   G_END_SEQUENCE = 0 And
   L_COPIES_PRINTED = 1 And
   L_PRINT = 1 Then
    Begin
    L_REPORT = "casmca820"
    Include "sfsmacro1.inc"
    End

L_CODE = "MCA825"
L_FORM = 0

Include "sfsbpend1.inc"

If L_STATE = 29 And
   L_PRINT = 1 Then
    Begin
    L_REPORT = "casmca825"
    Include "sfsmacro1.inc"
    End

L_CODE = "MCA322"
L_FORM = 0

Include "sfsbpend1.inc"

If L_STATE = 29 And
   L_PRINT = 1 Then
    Begin
    L_REPORT = "mca322_082005"
    Include "sfsmacro1.inc"
    End

If SFSCOMPANY:PRINT_ENDORSEMENTS = "Y" Then
    Begin
    Access SFPEND1, Set SFPEND1:POLICY_NO = G_POLICY_NO,
                        SFPEND1:POL_YEAR = G_YEAR,
                        SFPEND1:END_SEQUENCE = G_END_SEQUENCE, Approximate

L_MCA202 = "N"
L_MCA822 = "N"
L_MCA825 = "N"
l_mca301 = "N"
l_be23 = "N"
l_mca322 = "N"
    Repeat Begin
        Exit If SFPEND1:POLICY_NO <> G_POLICY_NO Or
                SFPEND1:POL_YEAR <> G_YEAR Or
                SFPEND1:END_SEQUENCE <> G_END_SEQUENCE

        -- MCA 301 - additional insured - lessor

        If SFPEND1:CODE = "MCA301" And
           l_mca301 = "N" Then
            Begin
            l_mca301 = "Y"
            L_PARAMETER = "casmca301.par"
            L_CODE = "MCA301"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmca301"
                Include "sfsmacro1.inc"
                End
            End

        -- MCA 104 - additional insured 

        If SFPEND1:CODE = "MCA104" And
           l_mca104 = "N" Then
            Begin
            l_mca104 = "Y"
            L_PARAMETER = "casmca301.par"
            L_CODE = "MCA104"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmca104"
                Include "sfsmacro1.inc"
                End
            End

        -- MCA 322 - mobile equipment 

        If SFPEND1:CODE = "MCA322" And
           l_mca322 = "N" Then
            Begin
            l_mca322 = "Y"
            L_PARAMETER = "casmca301.par"
            L_CODE = "MCA322"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmca322"
                Include "sfsmacro1.inc"
                End
            End

    If SFPEND1:OK_PRINT = "Y" Or
       CASDEFAUlT:DONT_PRINT_OPT_END = 0 or
       sfpend1:end_sequence = 0 Then
        Begin
        L_STATE = CAPVEHICLE:STATE
        L_LINE_OF_BUSINESS = CAPVEHICLE:LINE_OF_BUSINESS
        L_COMPANY_ID = CAPVEHICLE:COMPANY_ID

        -- MCA101 - additional insured volunteers

        If SFPEND1:CODE = "MCA101" Then
            Begin
            L_PARAMETER = "casmca301.par"
            L_CODE = "MCA101"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmca101"
                Include "sfsmacro1.inc"
                End
            End

        -- BE23 

        If SFPEND1:CODE = "BE23" Then
            Begin
            L_PARAMETER = "casmca301.par"
            L_CODE = "BE23"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "cppbe23"
                Include "sfsmacro1.inc"
                End
            End

        -- MCA 102 - employees as insureds

        If SFPEND1:CODE = "MCA102" Then
            Begin
            L_PARAMETER = "casmca301.par"
            L_CODE = "MCA102"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmca102"
                Include "sfsmacro1.inc"
                End
            End


        -- MCA 103 - Federal employees using autos in government business

        If SFPEND1:CODE = "MCA103" Then
            Begin
            L_PARAMETER = "casmca301.par"
            L_CODE = "MCA103"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmca103"
                Include "sfsmacro1.inc"
                End
            End

        -- MCA 110 - exlcusion of certain autos otherwise insured

        If SFPEND1:CODE = "MCA110" Then
            Begin
            L_PARAMETER = "casmca301.par"
            L_CODE = "MCA110"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmca110"
                Include "sfsmacro1.inc"
                End
            End

        -- MCA 111 - exclusion of certain autos otherwise insured

        If SFPEND1:CODE = "MCA111" Then
            Begin
            L_PARAMETER = "casmca301.par"
            L_CODE = "MCA111"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmca111"
                Include "sfsmacro1.inc"
                End
            End

        -- MCA 112 - excess coverage for certain autos otherwise insured

        If SFPEND1:CODE = "MCA112" Then
            Begin
            L_PARAMETER = "casmca301.par"
            L_CODE = "MCA112"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmca112"
                Include "sfsmacro1.inc"
                End
            End

        -- MCA 201 - audio, data and visual electronic equipment coverage

        If SFPEND1:CODE = "MCA201" Then
            Begin
            L_PARAMETER = "casmca301.par"
            L_CODE = "MCA201"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmca201"
                Include "sfsmacro1.inc"
                End
            End

        -- MCA 202 - rental reimbursement

        If SFPEND1:CODE = "MCA202" And
           L_MCA202 = "N" Then
            Begin
            L_MCA202 = "Y"
            L_PARAMETER = "casmca301.par"
            L_CODE = "MCA202"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmca202"
                Include "sfsmacro1.inc"
                End
            End

        -- MCA 203 - Stated amount coverage

        If SFPEND1:CODE = "MCA203" Then
            Begin
            L_PARAMETER = "casmca301.par"
            L_CODE = "MCA203"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmca203"
                Include "sfsmacro1.inc"
                End
            End

        -- MCA 302 - broadened coverage for named individuals

        If SFPEND1:CODE = "MCA302" Then
            Begin
            L_PARAMETER = "casmca301.par"
            L_CODE = "MCA302"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmca302"
                Include "sfsmacro1.inc"
                End
            End

        -- MCA 304 - covered auto symbol 10 description

        If SFPEND1:CODE = "MCA304" Then
            Begin
            L_PARAMETER = "casmca301.par"
            L_CODE = "MCA304"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmca304"
                Include "sfsmacro1.inc"
                End
            End

        -- MCA 305 - covered auto symbol 10 description - registration plates

        If SFPEND1:CODE = "MCA305" Then
            Begin
            L_PARAMETER = "casmca301.par"
            L_CODE = "MCA305"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmca305"
                Include "sfsmacro1.inc"
                End
            End

        -- MCA 306 - church/school buses

        If SFPEND1:CODE = "MCA306" Then
            Begin
            L_PARAMETER = "casmca301.par"
            L_CODE = "MCA306"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmca306"
                Include "sfsmacro1.inc"
                End
            End

        -- MCA 310 - suspension of insurance

        If SFPEND1:CODE = "MCA310" Then
            Begin
            L_PARAMETER = "casmca301.par"
            L_CODE = "MCA310"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmca310"
                Include "sfsmacro1.inc"
                End
            End

        -- MCA 311 - reinstatement of insurance

        If SFPEND1:CODE = "MCA311" Then
            Begin
            L_PARAMETER = "casmca301.par"
            L_CODE = "MCA311"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmca311"
                Include "sfsmacro1.inc"
                End
            End

        -- MCA 320 - Motor home coverage

        If SFPEND1:CODE = "MCA320" Then
            Begin
            L_PARAMETER = "casmca301.par"
            L_CODE = "MCA320"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmca320"
                Include "sfsmacro1.inc"
                End
            End

        -- MCA 321 - Motor home contents coverage

        If SFPEND1:CODE = "MCA321" Then
            Begin
            L_PARAMETER = "casmca301.par"
            L_CODE = "MCA321"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmca321"
                Include "sfsmacro1.inc"
                End
            End

        -- MCA 401 - Multistate Endorsement

        If SFPEND1:CODE = "MCA401" Then
            Begin
            L_PARAMETER = "casmca301.par"
            L_CODE = "MCA401"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmca401"
                Include "sfsmacro1.inc"
                End
            End

        -- MCA 601 - gap coverage

        If SFPEND1:CODE = "MCA601" Then
            Begin
            L_PARAMETER = "casmca301.par"
            L_CODE = "MCA601"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmca601"
                Include "sfsmacro1.inc"
                End
            End

          If SFPEND1:CODE = "MCA811" Then
            Begin
            L_PARAMETER = "casmca811.par"
            L_CODE = "MCA811"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
              Begin
                L_REPORT = "casmca811"
                Include "sfsmacro1.inc"
              End
            End

        --mca 812    Extraordinary Medical Benefits

        If SFPEND1:CODE = "MCA812" Then
            Begin
            L_PARAMETER = "casmca811.par"
            L_CODE = "MCA812"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
              Begin
                L_REPORT = "casmca812"
                Include "sfsmacro1.inc"
              End
            End

        -- MCA 822 - Additional PIP

        If SFPEND1:CODE = "MCA822" And
           L_MCA822 = "N" Then
            Begin
            L_MCA822 = "Y"
            L_PARAMETER = "casmca301.par"
            L_CODE = "MCA822"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmca822"
                Include "sfsmacro1.inc"
                End
            End

        -- MCA 825 

        If SFPEND1:CODE = "MCA825" And
           L_MCA825 = "N" Then
            Begin
            L_MCA825 = "Y"
            L_PARAMETER = "casmca301.par"
            L_CODE = "MCA825"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmca825"
                Include "sfsmacro1.inc"
                End
            End

        -- MCA 826 - medical expense benefits for bus passengers

        If SFPEND1:CODE = "MCA826" Then
            Begin
            L_PARAMETER = "casmca301.par"
            L_CODE = "MCA826"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmca826"
                Include "sfsmacro1.inc"
                End
            End

        -- MGK 010 - garagekeepers coverage form

        If SFPEND1:CODE = "MGK010" Then
            Begin
            L_PARAMETER = "casmca301.par"
            L_CODE = "MGK010"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmgk010"
                Include "sfsmacro1.inc"
                End
            End

        -- MGK 011 - garagekeepers supplemental form

        If SFPEND1:CODE = "MGK011" Then
            Begin
            L_PARAMETER = "casmca301.par"
            L_CODE = "MGK011"
            L_PREMIUM = SFPEND1:PREMIUM
            Include "sfsbpend.inc"

            If L_PRINT = 1 Then
                Begin
                L_REPORT = "casmgk011"
                Include "sfsmacro1.inc"
                End
            End

        End

        Next SFPEND1
        End

    End

Writefile(L_MACRO,"cli rm %s",Trun(L_MACRO))
Closefile(L_MACRO)
If SFSCOMPANY:DEBUG_MODE = 0 Then
    Proc("cli cq %s",Trun(L_MACRO))/noconsole
Else
    Proc("cli cq %s",Trun(L_MACRO))
