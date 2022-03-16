import React, { HTMLProps } from "react";
import { useTranslation } from "react-i18next";

const formStyle: React.CSSProperties = { marginTop: "1.5em" };
const fieldsetStyle: React.CSSProperties = {
  display: "flex",
  flexDirection: "column"
};

type Props = {
  fieldsetProperties?: HTMLProps<HTMLFieldSetElement>;
  formProperties?: HTMLProps<HTMLFormElement>;
  id: string;
  title?: string;
};

function SignIn({ fieldsetProperties, formProperties, id, title }: Props) {
  const { t } = useTranslation();
  return (
    <form
      style={formStyle}
      method="POST"
      action="/api/session"
      id={id}
      {...formProperties}
    >
      <fieldset style={fieldsetStyle} {...fieldsetProperties}>
        <legend>{t(title || "FORMS.SIGN_IN.TITLE")}</legend>
        <label htmlFor="FORMS.SIGN_IN.FIELDS.IDENTIFIER">
          {t("FORMS.SIGN_IN.FIELDS.IDENTIFIER.LABEL")}
        </label>
        <input
          type="text"
          name="login"
          id="FORMS.SIGN_IN.FIELDS.IDENTIFIER"
          required
        />
        <label htmlFor="FORMS.SIGN_IN.FIELDS.IDENTIFIER">
          {t("FORMS.SIGN_IN.FIELDS.PASSWORD.LABEL")}
        </label>
        <input
          type="password"
          name="password"
          id="FORMS.SIGN_IN.FIELDS.PASSWORD"
          required
        />
        <input
          type="submit"
          value={String(t("FORMS.SIGN_IN.FIELDS.SUBMIT.LABEL"))}
        />
      </fieldset>
    </form>
  );
}

export default SignIn;
